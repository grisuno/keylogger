#!/usr/bin/env bash
# =============================================================================
# install.sh - Prerequisite installer for the Linux keylogger project
# =============================================================================
# This script detects the Linux distribution, installs the required build
# tools (gcc, make, libc development headers), and adds the current user to
# the 'input' group so that /dev/input/event* can be accessed without root.
# It also verifies that the kernel supports evdev and that the required
# directories exist. Optionally, it compiles the keylogger after installation.
# =============================================================================

set -euo pipefail

# Colours for pretty output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Colour

# -----------------------------------------------------------------------------
# Helper functions
# -----------------------------------------------------------------------------
info()  { echo -e "${BLUE}[INFO]${NC} $1"; }
ok()    { echo -e "${GREEN}[ OK ]${NC} $1"; }
warn()  { echo -e "${YELLOW}[WARN]${NC} $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1" >&2; exit 1; }

# Check if we are running as root (or with sudo)
check_root() {
    if [[ $EUID -ne 0 ]]; then
        error "This script must be run as root (use sudo)."
    fi
}

# Detect the package manager and install packages
install_packages() {
    local packages=("$@")
    if command -v apt-get &>/dev/null; then
        info "Detected Debian/Ubuntu (apt). Installing: ${packages[*]}"
        apt-get update -qq
        apt-get install -y -qq "${packages[@]}"
    elif command -v dnf &>/dev/null; then
        info "Detected Fedora/RHEL 8+ (dnf). Installing: ${packages[*]}"
        dnf install -y -q "${packages[@]}"
    elif command -v yum &>/dev/null; then
        info "Detected RHEL/CentOS 7 (yum). Installing: ${packages[*]}"
        yum install -y -q "${packages[@]}"
    elif command -v zypper &>/dev/null; then
        info "Detected openSUSE (zypper). Installing: ${packages[*]}"
        zypper install -y -q "${packages[@]}"
    elif command -v pacman &>/dev/null; then
        info "Detected Arch Linux (pacman). Installing: ${packages[*]}"
        pacman -Sy --noconfirm "${packages[@]}"
    elif command -v apk &>/dev/null; then
        info "Detected Alpine Linux (apk). Installing: ${packages[*]}"
        apk add --no-cache "${packages[@]}"
    else
        error "No supported package manager found. Please install gcc, make, and libc-dev manually."
    fi
}

# -----------------------------------------------------------------------------
# Main installation routine
# -----------------------------------------------------------------------------
main() {
    check_root

    info "Starting prerequisite installation for keylogger project."

    # 1. Determine the set of packages needed per distribution family
    local pkgs=()
    if command -v apt-get &>/dev/null; then
        pkgs=(build-essential gcc make)
        # On Debian/Ubuntu, libc6-dev is part of build-essential, so no extra
    elif command -v dnf &>/dev/null || command -v yum &>/dev/null; then
        pkgs=(gcc make glibc-devel)
    elif command -v zypper &>/dev/null; then
        pkgs=(gcc make glibc-devel)
    elif command -v pacman &>/dev/null; then
        pkgs=(gcc make glibc)   # Arch: base-devel includes gcc, make; glibc provides headers
    elif command -v apk &>/dev/null; then
        pkgs=(gcc make musl-dev) # Alpine uses musl libc
    else
        error "Unsupported distribution. Please install gcc and make manually."
    fi

    # 2. Install the packages
    install_packages "${pkgs[@]}"
    ok "Build tools installed."

    # 3. Ensure /dev/input exists and is accessible
    if [[ ! -d /dev/input ]]; then
        warn "/dev/input does not exist. Your kernel may not have evdev support, or the module is not loaded."
        warn "Try: modprobe evdev"
    else
        ok "/dev/input directory found."
    fi

    # 4. Add the original user (who invoked sudo) to the 'input' group
    #    The SUDO_USER environment variable is set when using sudo.
    if [[ -n "${SUDO_USER:-}" ]]; then
        local real_user="$SUDO_USER"
        info "Adding user '$real_user' to the 'input' group."
        if getent group input &>/dev/null; then
            usermod -a -G input "$real_user"
            ok "User '$real_user' added to group 'input'."
            warn "You must log out and back in (or start a new session) for group changes to take effect."
        else
            warn "Group 'input' does not exist on this system. Creating it..."
            groupadd input
            usermod -a -G input "$real_user"
            ok "Group 'input' created and user added."
        fi
    else
        warn "SUDO_USER not set. If you are not running with sudo, you may need to add yourself to the 'input' group manually."
        warn "Command: sudo usermod -a -G input $USER"
    fi

    # 5. Check if the user can read /dev/input/event* (test with the first event device)
    if [[ -e /dev/input/event0 ]]; then
        if [[ -r /dev/input/event0 ]]; then
            ok "Current root can read /dev/input/event0."
        else
            warn "/dev/input/event0 exists but is not readable by root (unusual)."
        fi
    else
        warn "No event device found at /dev/input/event0. Plug in a keyboard and check."
    fi

    # 6. Optional: compile the keylogger if the source file exists
    if [[ -f "keylogger.c" ]]; then
        read -p "Do you want to compile the keylogger now? [y/N] " -r
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            if command -v make &>/dev/null; then
                info "Running 'make' to compile..."
                make
                if [[ -f "keylogger" ]]; then
                    ok "Compilation successful. Executable: ./keylogger"
                else
                    warn "Compilation may have failed. Check output above."
                fi
            else
                warn "Make not found; please run 'make' manually after installation."
            fi
        else
            info "Skipping compilation. You can run 'make' later."
        fi
    else
        warn "keylogger.c not found in current directory. Ensure the source file is present before compiling."
    fi

    # 7. Final summary
    echo ""
    ok "Prerequisite installation completed."
    echo ""
    echo -e "${CYAN}Next steps:${NC}"
    echo "  1. Log out and back in (or run 'newgrp input') to activate the 'input' group."
    echo "  2. Compile: make"
    echo "  3. Run: make run   (or sudo ./keylogger)"
    echo "  4. View log: make read"
    echo "  5. Stop: make stop"
    echo ""
    echo -e "${YELLOW}Remember: This tool is for authorized testing only.${NC}"
}

# -----------------------------------------------------------------------------
# Execute main
# -----------------------------------------------------------------------------
main "$@"