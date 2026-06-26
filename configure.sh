#!/bin/sh
#
# configure - Environment verification script for the Linux keylogger
#             (evdev-based) project.
#
# Usage: ./configure [--prefix=PREFIX] [--help]
#
# Checks:
#   - C compiler (gcc or cc)
#   - make utility
#   - Standard C library headers (stdio.h, stdlib.h, etc.)
#   - Linux input header <linux/input.h> and <linux/input-event-codes.h>
#   - /dev/input directory and at least one event device
#   - Current user's ability to read /dev/input/event* (root or input group)
#   - That the source file keylogger.c exists
#
# Generates config.mk with:
#   - CC, CFLAGS, LDFLAGS
#   - HAVE_INPUT_HEADER (yes/no)
#   - NEED_SUDO (yes/no) based on group membership
#   - INSTALL_PREFIX
#   - LOG_FILE location (can be overridden)
#
# This script is idempotent and can be re-run to update configuration.
#
# -----------------------------------------------------------------------------

set -u

# Default values
PREFIX="/usr/local"
LOG_FILE="/tmp/.keylog"
CFLAGS="-Wall -Wextra -O2 -pedantic"
LDFLAGS="-lrt"
CC="${CC:-gcc}"
MAKE="${MAKE:-make}"
SOURCE="keylogger.c"

# Colours (if terminal supports)
if test -t 1; then
    RED=$(printf '\033[0;31m')
    GREEN=$(printf '\033[0;32m')
    YELLOW=$(printf '\033[0;33m')
    BLUE=$(printf '\033[0;34m')
    NC=$(printf '\033[0m')
else
    RED=""; GREEN=""; YELLOW=""; BLUE=""; NC=""
fi

# -----------------------------------------------------------------------------
# Helper functions
# -----------------------------------------------------------------------------
msg_info()  { printf "${BLUE}%s${NC}\n" "$1"; }
msg_ok()    { printf "${GREEN}%s${NC}\n" "$1"; }
msg_warn()  { printf "${YELLOW}%s${NC}\n" "$1" >&2; }
msg_error() { printf "${RED}%s${NC}\n" "$1" >&2; }

# Check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if a C header exists by trying to compile a tiny program
check_header() {
    local header="$1"
    local code="#include <$header>\nint main(void){ return 0; }"
    echo "$code" | $CC -c -x c - -o /dev/null >/dev/null 2>&1
}

# Check if we can read a device file (by opening it)
can_read_device() {
    local dev="$1"
    test -r "$dev" 2>/dev/null
}

# -----------------------------------------------------------------------------
# Parse command-line arguments
# -----------------------------------------------------------------------------
while test $# -gt 0; do
    case "$1" in
        --prefix=*)
            PREFIX="${1#*=}"
            ;;
        --prefix)
            shift
            PREFIX="$1"
            ;;
        --help|-h)
            cat <<EOF
Usage: $0 [OPTIONS]

Options:
  --prefix=PREFIX   Installation prefix (default: /usr/local)
  --help, -h        Show this help message

This script checks that all prerequisites are met to build and install
the keylogger. It does not modify the system; it only verifies and
writes a config.mk file for the Makefile.
EOF
            exit 0
            ;;
        *)
            msg_error "Unknown option: $1"
            exit 1
            ;;
    esac
    shift
done

# -----------------------------------------------------------------------------
# Start checks
# -----------------------------------------------------------------------------
msg_info "Checking build environment for keylogger..."

# 1. Check for source file
if test ! -f "$SOURCE"; then
    msg_error "Source file '$SOURCE' not found in current directory."
    exit 1
fi
msg_ok "Source file '$SOURCE' present."

# 2. Check for C compiler
if command_exists "$CC"; then
    msg_ok "C compiler: $CC"
else
    # Try cc
    if command_exists cc; then
        CC="cc"
        msg_ok "C compiler: $CC"
    else
        msg_error "No C compiler found. Please install gcc or clang."
        exit 1
    fi
fi

# 3. Check for 'make'
if command_exists "$MAKE"; then
    msg_ok "make utility: $MAKE"
else
    msg_error "make not found. Please install make."
    exit 1
fi

# 4. Check basic libc headers (stdio, stdlib, string, unistd)
for hdr in stdio.h stdlib.h string.h unistd.h; do
    if check_header "$hdr"; then
        : # ok
    else
        msg_error "Header '$hdr' not found. Install libc development package."
        exit 1
    fi
done
msg_ok "Standard C headers available."

# 5. Check Linux input headers (linux/input.h, linux/input-event-codes.h)
if check_header "linux/input.h"; then
    msg_ok "linux/input.h found."
else
    msg_error "linux/input.h not found. Install kernel headers (linux-libc-dev or kernel-devel)."
    exit 1
fi

if check_header "linux/input-event-codes.h"; then
    msg_ok "linux/input-event-codes.h found."
else
    # On older kernels, this might be included inside input.h, so not fatal
    msg_warn "linux/input-event-codes.h not found (may be optional)."
fi

# 6. Check that /dev/input exists and has event devices
if test -d /dev/input; then
    msg_ok "/dev/input directory exists."
    # Find at least one event device
    found_dev=0
    for dev in /dev/input/event*; do
        if test -e "$dev"; then
            found_dev=1
            break
        fi
    done
    if test $found_dev -eq 1; then
        msg_ok "At least one event device found (e.g., $dev)."
    else
        msg_error "No /dev/input/event* devices found. Is evdev loaded? (try: modprobe evdev)"
        exit 1
    fi
else
    msg_error "/dev/input does not exist. Your kernel may not support evdev."
    exit 1
fi

# 7. Check read access to event devices (for the current user)
# We test /dev/input/event0 if present, otherwise try the first found.
first_dev=""
for dev in /dev/input/event*; do
    if test -e "$dev"; then
        first_dev="$dev"
        break
    fi
done

need_sudo="yes"
if test -n "$first_dev"; then
    if can_read_device "$first_dev"; then
        msg_ok "Current user can read $first_dev (no sudo needed)."
        need_sudo="no"
    else
        # Try to see if we are in the 'input' group by checking group membership
        if groups | grep -q "\binput\b" 2>/dev/null; then
            # But still may not have read access due to permissions? Actually if in input group, should be able.
            # We already tested read; if it failed, maybe permissions are 660 root:input and we are not root.
            # We'll warn.
            msg_warn "User is in 'input' group but cannot read $first_dev. Check device permissions (should be 660 root:input)."
            need_sudo="yes"
        else
            msg_warn "User not in 'input' group and cannot read $first_dev. Will need sudo for execution."
            need_sudo="yes"
        fi
    fi
else
    msg_warn "No event device found for testing; assuming sudo may be required."
    need_sudo="yes"
fi

# 8. Check if we can write to /tmp (for log file)
if test -w /tmp; then
    msg_ok "Can write to /tmp (log directory)."
else
    msg_warn "Cannot write to /tmp; log will be written to $LOG_FILE but may fail at runtime."
    # We can change to /var/log but we'll keep as is.
fi

# -----------------------------------------------------------------------------
# Generate config.mk
# -----------------------------------------------------------------------------
msg_info "Writing config.mk..."

cat > config.mk <<EOF
# config.mk - Generated by configure
# Do not edit manually; re-run configure to update.

# Compiler and flags
CC = $CC
CFLAGS = $CFLAGS
LDFLAGS = $LDFLAGS

# Installation prefix
PREFIX = $PREFIX

# Log file location (can be overridden)
LOG_FILE = $LOG_FILE

# Whether sudo is needed for 'make run' and 'make stop'
NEED_SUDO = $need_sudo

# Indicate that input headers are available (always yes if we passed)
HAVE_INPUT_HEADER = yes

# Additional defines (if any)
DEFS = -DLOG_FILE=\\"$LOG_FILE\\"
EOF

msg_ok "config.mk generated successfully."

# -----------------------------------------------------------------------------
# Summary
# -----------------------------------------------------------------------------
echo ""
msg_info "Configuration summary:"
echo "  Installation prefix: $PREFIX"
echo "  Compiler:           $CC"
echo "  Log file:           $LOG_FILE"
echo "  Sudo required:      $need_sudo"
echo ""
msg_ok "All checks passed. You can now run 'make' to build, and 'make install' to install."

exit 0