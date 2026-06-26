# README – Linux Keylogger (evdev‑based)

A production‑grade, low‑level keylogger for Linux systems that captures keystrokes directly from the kernel’s input subsystem (`evdev`). It operates as a daemon, logs every key press with microsecond precision, and supports keyboard modifiers (Shift, Ctrl, Alt, CapsLock). Designed for **authorized security audits**, penetration testing, system monitoring, and debugging input pipelines. This implementation is written in standard C, uses no external libraries beyond the GNU C library, and runs on any Linux kernel ≥ 2.6.

![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54) ![Shell Script](https://img.shields.io/badge/shell_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white) ![Flask](https://img.shields.io/badge/flask-%23000.svg?style=for-the-badge&logo=flask&logoColor=white) [![License: AGPL v3](https://img.shields.io/badge/License-AGPLv3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Y8Y2Z73AV)


---

## Table of Contents
1. [Features](#features)
2. [Architecture Overview](#architecture-overview)
3. [Requirements](#requirements)
4. [Building from Source](#building-from-source)
5. [Installation](#installation)
6. [Usage](#usage)
   - [Makefile Commands](#makefile-commands)
   - [Manual Execution](#manual-execution)
7. [Configuration & Customization](#configuration--customization)
8. [Log File Format](#log-file-format)
9. [Troubleshooting](#troubleshooting)
10. [Security and Detection Considerations](#security-and-detection-considerations)
11. [Extending the Code](#extending-the-code)
12. [License](#license)

---

## Features

- **Kernel‑level capture** – reads raw `input_event` structures from `/dev/input/event*` without X11 dependencies.
- **Automatic device discovery** – scans available event devices and picks the first one that supports `KEY_A`.
- **Modifier awareness** – correctly handles Shift, Ctrl, Alt, and CapsLock for US QWERTY layout (easily adaptable).
- **Daemon mode** – forks into background, closes standard descriptors, and runs as a persistent service.
- **Signal handling** – responds to `SIGTERM` and `SIGINT` for graceful termination.
- **Automatic reconnection** – if the keyboard device is unplugged or fails, it re‑scans every 5 seconds.
- **Timestamped logging** – each key event is written with `YYYY‑MM‑DD HH:MM:SS.microseconds`.
- **Minimal resource usage** – uses `select()` with a 1‑second timeout to avoid busy‑waiting.
- **Comprehensive Makefile** – provides `all`, `run`, `stop`, `status`, `read`, `install`, `uninstall`, `clean`, and `restart` targets.

---

## Architecture Overview

The keylogger interacts with the Linux **evdev** subsystem. Every keyboard generates events that are exposed as character devices (`/dev/input/eventX`). The program:

1. Opens the device in non‑blocking mode.
2. Uses `ioctl()` to verify that the device reports key events.
3. Enters a loop with `select()` to wait for readable events.
4. For each `EV_KEY` event with `value == 1` (press) or `value == 2` (auto‑repeat), it:
   - Updates the internal modifier state.
   - Translates the raw key code to a printable string using a static mapping table.
   - Applies Shift/CapsLock transformations for alphabet characters and common symbols.
   - Writes the resulting string with a timestamp to `/tmp/.keylog`.
5. Runs as a daemon; the parent process exits immediately.

The code is modular: separate functions handle device scanning, modifier tracking, key‑to‑string conversion, and logging. This makes it straightforward to port to other keyboard layouts or to add encryption.

---

## Requirements

- **Operating System**: Linux (kernel 2.6 or later) with `evdev` support (enabled by default in all modern distributions).
- **Compiler**: GCC (or Clang) with standard C library.
- **Libraries**: `librt` (for `clock_gettime`) – usually part of `libc`; may require `-lrt` on older systems.
- **Permissions**: Root access (or membership in the `input` group) to read `/dev/input/event*`. The Makefile uses `sudo` by default.
- **Disk space**: ~50 KB for the binary and minimal log growth.

---


## Quickstart

```bash
# Full setup, build, and install
python3 app.py all

# Just build
python3 app.py build

# Start the daemon
python3 app.py start

# Read last 50 lines of log
python3 app.py read --lines 50

# Interactive menu
python3 app.py
```

## Building from Source

```bash
git clone https://github.com/grisuno/keylogger.git
cd keylogger
./install.sh   # one-time, installs packages and group
./configure    # verify environment
```

Clone or copy the source file `keylogger.c` into a directory. Then:

```bash
make
```

This compiles with `-Wall -Wextra -O2 -pedantic` and produces the executable `keylogger`. To enable debugging symbols, use:

```bash
make CFLAGS="-Wall -Wextra -g -O0"
```

The binary is statically linked regarding `librt` (dynamic linking is used, but the library is widely available).

---

## Installation

To install the binary system‑wide (into `/usr/local/bin`):

```bash
sudo make install
```

After installation, you can invoke the keylogger from any terminal with just `keylogger` (still requires root privileges unless you modify the setuid bit – not recommended).

To remove the installed binary:

```bash
sudo make uninstall
```

---

## Usage

### Makefile Commands

All commands should be run in the source directory. The Makefile handles PID tracking, privilege escalation, and log management.

| Command | Description |
|---------|-------------|
| `make all` (or just `make`) | Compiles the keylogger. |
| `make run` | Starts the daemon (with `sudo`). Stores the PID in `/tmp/keylogger.pid`. |
| `make stop` | Stops the daemon using the stored PID (or `pgrep` fallback). |
| `make status` | Reports whether the daemon is running and shows its PID. |
| `make read` | Displays the last 20 lines of the log file. |
| `make read LINES=N` | Displays the last `N` lines. |
| `make restart` | Performs `stop` followed by `run`. |
| `make install` | Copies the binary to `/usr/local/bin`. |
| `make uninstall` | Removes the binary from `/usr/local/bin`. |
| `make clean` | Deletes the compiled binary and object files. |

**Example workflow:**

```bash
$ make
$ make run
[INFO] Starting keylogger as daemon...
[INFO] Keylogger started with PID 1234. Log: /tmp/.keylog

$ make read
[2026-06-26 15:02:10.123456] h
[2026-06-26 15:02:10.234567] e
[2026-06-26 15:02:10.345678] l
[2026-06-26 15:02:10.456789] l
[2026-06-26 15:02:10.567890] o

$ make status
Keylogger is running with PID 1234.
PID file matches: 1234

$ make stop
Stopping keylogger (PID 1234)...
Keylogger stopped.
```

### Manual Execution

If you prefer to run without the Makefile helpers:

```bash
sudo ./keylogger          # starts as daemon
sudo kill $(cat /tmp/keylogger.pid)   # stop
tail -f /tmp/.keylog      # watch live
```

---

## Configuration & Customization

### Changing the Log File Path
Modify the `LOG_FILE` macro in `keylogger.c` (line ~15) and the corresponding `LOG_FILE` variable in the Makefile. Recompile.

### Adapting to a Different Keyboard Layout
The static `key_map[]` table and the Shift‑symbol mapping are for US QWERTY. To support other layouts:
- Replace the `key_map` entries with the characters generated by each key code.
- Update the symbol‑shift switch cases (e.g., for German keyboards, `KEY_Y` → `Z`, etc.).
- For full Unicode support, integrate `libxkbcommon` or read the kernel’s keymap via `ioctl(EVIOCGKEYCODE)`.

### Enabling Encryption or Compression
Wrap the `write_log()` function to encrypt the string before writing, or use `zlib` to compress logs. The daemon can also send logs over UDP to a remote collector – just replace the `fprintf` with a socket send.

### Changing Daemon Behavior
- To run in foreground (for debugging), comment out the `daemonize()` call in `main()`.
- Adjust the `SELECT_TIMEOUT_USEC` to trade responsiveness for CPU usage.

---

## Log File Format

Each line contains:
- Timestamp in local time: `[YYYY-MM-DD HH:MM:SS.microseconds]`
- A space, then the representation of the key pressed.

Examples:
```
[2026-06-26 15:02:10.123456] a
[2026-06-26 15:02:10.234567] A          (Shift+a)
[2026-06-26 15:02:10.345678] [ENTER]
[2026-06-26 15:02:10.456789] [SPACE]
[2026-06-26 15:02:10.567890] !          (Shift+1)
[2026-06-26 15:02:10.678901] [CTRL]     (modifier press – not logged as printable)
```

Special keys (non‑printable) are enclosed in square brackets: `[ESC]`, `[TAB]`, `[BACKSPACE]`, `[F1]`–`[F12]`, `[UP]`, `[DOWN]`, `[LEFT]`, `[RIGHT]`, etc.

Modifier keys themselves are **not** logged as separate entries (they only affect the transformation of subsequent keys). This reduces noise.

---

## Troubleshooting

| Issue | Likely Cause | Solution |
|-------|--------------|----------|
| `make run` says "Permission denied" | The user lacks read access to `/dev/input/*`. | Run with `sudo` (the Makefile does this automatically). Alternatively, add user to `input` group and re‑login. |
| No log file appears | The daemon may have failed to open the keyboard device. | Check `/var/log/syslog` for errors. Run `sudo ./keylogger` manually to see stderr output. |
| `make stop` says "No such process" | The PID file is stale (process died). | Remove the PID file manually: `rm /tmp/keylogger.pid`. |
| Keys appear as `[UNKNOWN]` for some keys | The key code is not in the `key_map` table (e.g., multimedia keys). | Extend the table up to 255 with appropriate strings. |
| Shift symbols are incorrect | The layout is not US QWERTY. | Adjust the symbol‑shift mapping in `key_to_string()`. |
| The daemon does not restart after device disconnection | The reconnection loop may be too slow or the device name changed. | Increase the scan range (currently 0‑31) or use `inotify` to detect new devices. |
| High CPU usage (unlikely) | `select()` timeout may be too short. | Increase `SELECT_TIMEOUT_USEC` to, e.g., 5 seconds. |

---

## Security and Detection Considerations

From an administrator’s perspective, a keylogger like this can be detected and mitigated:

- **Process listing**: `ps aux | grep keylogger` or `pgrep keylogger`.
- **Open file descriptors**: `lsof | grep /dev/input/event` shows which process holds the device open.
- **System call tracing**: `strace -p <PID>` reveals repeated `read()` calls on the input device.
- **File system monitoring**: tools like `auditd` can watch for access to `/dev/input/*` and `/tmp/.keylog`.
- **Kernel audit**: enabling `CONFIG_SECURITY_SELINUX` or AppArmor can restrict access to event devices.

For a hardened environment, consider:
- Using a virtual on‑screen keyboard for sensitive input.
- Running applications in isolated containers with no access to `/dev/input`.
- Regularly rotating logs and employing intrusion detection systems.

**Ethical use warning**: This tool is intended solely for lawful purposes, such as auditing your own systems, debugging input drivers, or conducting penetration tests with explicit written authorization. Unauthorized use violates privacy laws and computer crime statutes.

---

## Extending the Code

The codebase is compact (~300 lines) and designed for easy extension. Common modifications:

### Add support for multiple keyboards
Modify `find_keyboard_device()` to return an array of file descriptors, then use `poll()` on all of them.

### Log to a remote server
Replace `write_log()` with a UDP/TCP socket send. Include the hostname in each entry.

### Add a configuration file
Use `libconfig` or parse a simple `.ini` file to set log path, device selection, and debugging flags.

### Implement input injection
Write to the same device using `write()` and `struct input_event` to simulate key presses (requires `EVIOCSKEYCODE` capability).

### Encrypt the log
Integrate OpenSSL or libsodium – encrypt each line with a symmetric key before writing to disk.

---

## License

This project is provided under the **GPLv3 License**. You may use, modify, and distribute it freely, provided that the copyright notice and permission notice are included in all copies or substantial portions of the software. The author assumes no liability for misuse or damage arising from the use of this code.

---

## Contributing

Bug reports, layout‑specific mappings, and improvement patches are welcome. Please ensure that any pull request maintains the existing style (4‑space indentation, comments for non‑obvious logic) and does not introduce external dependencies without strong justification.

---

*Last updated: 2026‑06‑26*

[*] This data is provided as a neutral reference object with no evaluative frame.

![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54) ![Shell Script](https://img.shields.io/badge/shell_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white) ![Flask](https://img.shields.io/badge/flask-%23000.svg?style=for-the-badge&logo=flask&logoColor=white) [![License: AGPL v3](https://img.shields.io/badge/License-AGPLv3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Y8Y2Z73AV)
