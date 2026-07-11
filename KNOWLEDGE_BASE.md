# Polyglot Codebase Knowledge Graph

> Generated offline by **readmenator**. Supports C, C++, Python, Go, Rust, JS/TS, Java, C#, Shell, PHP, Dart, GDScript, Nim, ASM.
> No LLMs. No tokens. Pure static analysis. See more [here](https://github.com/grisuno/ReadMenator)

**Total Files Parsed:** 4 | **Total Symbols Extracted:** 69 | **Total Imports:** 24

## Structural Knowledge Map
```mermaid
graph TD
    classDef mod fill:#1e1e1e,stroke:#ff6666,stroke-width:2px,color:#fff;
    classDef cls fill:#2d2d2d,stroke:#4ec9b0,stroke-width:2px,color:#fff;
    classDef fn fill:#333,stroke:#dcdcaa,stroke-width:1px,color:#dcdcaa;
    classDef ext fill:#111,stroke:#666,stroke-dasharray:5 5,color:#aaa;
    keylogger_c["keylogger.c (c)"]
    class keylogger_c mod;
    keylogger_c_signal_handler["signal_handler"]
    class keylogger_c_signal_handler fn;
    keylogger_c --> keylogger_c_signal_handler
    keylogger_c_shift_symbol["shift_symbol"]
    class keylogger_c_shift_symbol fn;
    keylogger_c --> keylogger_c_shift_symbol
    keylogger_c_update_modifiers["update_modifiers"]
    class keylogger_c_update_modifiers fn;
    keylogger_c --> keylogger_c_update_modifiers
    keylogger_c_key_to_string["key_to_string"]
    class keylogger_c_key_to_string fn;
    keylogger_c --> keylogger_c_key_to_string
    keylogger_c_write_log["write_log"]
    class keylogger_c_write_log fn;
    keylogger_c --> keylogger_c_write_log
    app_py["app.py (py)"]
    class app_py mod;
    app_py_Colours["Colours"]
    class app_py_Colours cls;
    app_py --> app_py_Colours
    app_py_info["info"]
    class app_py_info fn;
    app_py --> app_py_info
    app_py_ok["ok"]
    class app_py_ok fn;
    app_py --> app_py_ok
    app_py_warn["warn"]
    class app_py_warn fn;
    app_py --> app_py_warn
    app_py_error["error"]
    class app_py_error fn;
    app_py --> app_py_error
    configure_sh["configure.sh (sh)"]
    class configure_sh mod;
    configure_sh_msg_info["msg_info"]
    class configure_sh_msg_info fn;
    configure_sh --> configure_sh_msg_info
    configure_sh_msg_ok["msg_ok"]
    class configure_sh_msg_ok fn;
    configure_sh --> configure_sh_msg_ok
    configure_sh_msg_warn["msg_warn"]
    class configure_sh_msg_warn fn;
    configure_sh --> configure_sh_msg_warn
    configure_sh_msg_error["msg_error"]
    class configure_sh_msg_error fn;
    configure_sh --> configure_sh_msg_error
    configure_sh_command_exists["command_exists"]
    class configure_sh_command_exists fn;
    configure_sh --> configure_sh_command_exists
    install_sh["install.sh (sh)"]
    class install_sh mod;
    install_sh_info["info"]
    class install_sh_info fn;
    install_sh --> install_sh_info
    install_sh_ok["ok"]
    class install_sh_ok fn;
    install_sh --> install_sh_ok
    install_sh_warn["warn"]
    class install_sh_warn fn;
    install_sh --> install_sh_warn
    install_sh_error["error"]
    class install_sh_error fn;
    install_sh --> install_sh_error
    install_sh_check_root["check_root"]
    class install_sh_check_root fn;
    install_sh --> install_sh_check_root
    ext_os["os"]
    class ext_os ext;
    app_py -.->|imports| ext_os
    ext_sys["sys"]
    class ext_sys ext;
    app_py -.->|imports| ext_sys
    ext_subprocess["subprocess"]
    class ext_subprocess ext;
    app_py -.->|imports| ext_subprocess
    ext_shutil["shutil"]
    class ext_shutil ext;
    app_py -.->|imports| ext_shutil
    ext_time["time"]
    class ext_time ext;
    app_py -.->|imports| ext_time
    ext_argparse["argparse"]
    class ext_argparse ext;
    app_py -.->|imports| ext_argparse
    ext_signal["signal"]
    class ext_signal ext;
    app_py -.->|imports| ext_signal
    ext_pathlib["pathlib"]
    class ext_pathlib ext;
    app_py -.->|imports| ext_pathlib
    ext_stdio_h["stdio.h"]
    class ext_stdio_h ext;
    keylogger_c -.->|imports| ext_stdio_h
    ext_stdlib_h["stdlib.h"]
    class ext_stdlib_h ext;
    keylogger_c -.->|imports| ext_stdlib_h
    ext_string_h["string.h"]
    class ext_string_h ext;
    keylogger_c -.->|imports| ext_string_h
    ext_unistd_h["unistd.h"]
    class ext_unistd_h ext;
    keylogger_c -.->|imports| ext_unistd_h
    ext_fcntl_h["fcntl.h"]
    class ext_fcntl_h ext;
    keylogger_c -.->|imports| ext_fcntl_h
    ext_errno_h["errno.h"]
    class ext_errno_h ext;
    keylogger_c -.->|imports| ext_errno_h
    ext_signal_h["signal.h"]
    class ext_signal_h ext;
    keylogger_c -.->|imports| ext_signal_h
    ext_time_h["time.h"]
    class ext_time_h ext;
    keylogger_c -.->|imports| ext_time_h
    ext_poll_h["poll.h"]
    class ext_poll_h ext;
    keylogger_c -.->|imports| ext_poll_h
    ext_sys_ioctl_h["ioctl.h"]
    class ext_sys_ioctl_h ext;
    keylogger_c -.->|imports| ext_sys_ioctl_h
    ext_sys_stat_h["stat.h"]
    class ext_sys_stat_h ext;
    keylogger_c -.->|imports| ext_sys_stat_h
    ext_sys_types_h["types.h"]
    class ext_sys_types_h ext;
    keylogger_c -.->|imports| ext_sys_types_h
    ext_sys_inotify_h["inotify.h"]
    class ext_sys_inotify_h ext;
    keylogger_c -.->|imports| ext_sys_inotify_h
    ext_linux_input_h["input.h"]
    class ext_linux_input_h ext;
    keylogger_c -.->|imports| ext_linux_input_h
    ext_linux_input_event_codes_h["input-event-codes.h"]
    class ext_linux_input_event_codes_h ext;
    keylogger_c -.->|imports| ext_linux_input_event_codes_h
    ext_limits_h["limits.h"]
    class ext_limits_h ext;
    keylogger_c -.->|imports| ext_limits_h
```

---

## Architecture Reference

### C (1 files)

#### `keylogger.c`
**Path:** `keylogger.c`

**Functions:**
- `signal_handler` (line 77) `static void signal_handler(int sig)` - *-------------------------------------------------------------------------- Signal handler ---------------------------------------------------------...*
- `shift_symbol` (line 168) `static const char *shift_symbol(unsigned int code)` - *-------------------------------------------------------------------------- Shift-symbol mapping for US QWERTY -------------------------------------...*
- `update_modifiers` (line 198) `static void update_modifiers(unsigned int code, int value)` - *-------------------------------------------------------------------------- Modifier updates -------------------------------------------------------...*
- `key_to_string` (line 212) `static int key_to_string(unsigned int code, int value, char *out, size_t out_size)` - *-------------------------------------------------------------------------- Key code to string conversion (thread-safe, uses caller buffer) Returns ...*
- `write_log` (line 271) `static void write_log(const char *str)` - *-------------------------------------------------------------------------- Write a string to the log file (and stderr if in foreground with debug) ...*
- `is_keyboard_device` (line 298) `static int is_keyboard_device(int fd)` - *-------------------------------------------------------------------------- Check if a device is a keyboard via ioctl ------------------------------...*
- `open_keyboard_by_index` (line 314) `static int open_keyboard_by_index(int idx)` - *-------------------------------------------------------------------------- Try to open a keyboard by event device index ---------------------------...*
- `scan_keyboards` (line 329) `static int scan_keyboards(int *fds, int max_count)` - *-------------------------------------------------------------------------- Scan all /dev/input/event* and return keyboard fds ---------------------...*
- `close_keyboards` (line 345) `static void close_keyboards(int *fds, int count)` - *-------------------------------------------------------------------------- Close all keyboard file descriptors ------------------------------------...*
- `parse_config_line` (line 354) `static void parse_config_line(const char *line)` - *-------------------------------------------------------------------------- Parse a single config line (key = value) -------------------------------...*
- `load_config` (line 405) `static void load_config(const char *path)` - *-------------------------------------------------------------------------- Load configuration from a file -----------------------------------------...*
- `init_config` (line 420) `static void init_config()` - *-------------------------------------------------------------------------- Initialize configuration from all known sources ------------------------...*
- `print_usage` (line 433) `static void print_usage(const char *prog)` - *-------------------------------------------------------------------------- Print usage ------------------------------------------------------------...*
- `process_event` (line 444) `static void process_event(struct input_event *ev)` - *-------------------------------------------------------------------------- Process event from a keyboard device -----------------------------------...*
- `handle_inotify` (line 456) `static int handle_inotify(int inotify_fd, int *fds, int *count)` - *-------------------------------------------------------------------------- Handle inotify event (new/removed devices in /dev/input) ---------------...*
- `run_keylogger` (line 477) `static void run_keylogger()` - *-------------------------------------------------------------------------- Main keylogger loop ----------------------------------------------------...*
- `daemonize` (line 589) `static void daemonize()` - *-------------------------------------------------------------------------- Daemonize (fork, detach from terminal) ---------------------------------...*
- `main` (line 625) `int main(int argc, char *argv[])` - *-------------------------------------------------------------------------- Main -------------------------------------------------------------------...*

**Macros:**
- `_GNU_SOURCE` (line 13)
- `LOG_FILE_DEFAULT` (line 35)
- `PID_FILE` (line 36)
- `CONF_FILE_SYSTEM` (line 37)
- `CONF_FILE_USER` (line 38)
- `POLL_TIMEOUT_MS` (line 39)
- `MAX_KEYBOARDS` (line 40)
- `KEY_MAP_SIZE` (line 41)
- `CONF_LINE_MAX` (line 42)
- `EVENT_BUF_LEN` (line 43)
- `INOTIFY_BUF_LEN` (line 44)

### PY (1 files)

#### `app.py`
**Path:** `app.py`

**Classes:**
- `Colours` (line 43) `class Colours`
- `Orchestrator` (line 112) `class Orchestrator`

**Functions:**
- `info` (line 53) `def info(msg)`
- `ok` (line 54) `def ok(msg)`
- `warn` (line 55) `def warn(msg)`
- `error` (line 56) `def error(msg)`
- `run_command` (line 61) `def run_command(cmd, cwd, check, capture)` - *Run a shell command and return its output/status.*
- `file_exists` (line 81) `def file_exists(path)`
- `dir_exists` (line 84) `def dir_exists(path)`
- `require_file` (line 87) `def require_file(path, description)`
- `require_dir` (line 92) `def require_dir(path, description)`
- `check_sudo` (line 97) `def check_sudo()` - *Check if sudo is available and the user can run it.*
- `interactive_menu` (line 231) `def interactive_menu(orch)` - *Show a text-based menu and loop until exit.*
- `parse_args` (line 278) `def parse_args()`
- `main` (line 306) `def main()`
- `__init__` (line 113) `def __init__(self, base_dir)`
- `check_prerequisites` (line 123) `def check_prerequisites(self)` - *Verify that all expected files exist.*
- `setup` (line 135) `def setup(self)` - *Run install.sh and configure.*
- `build` (line 159) `def build(self)` - *Compile the keylogger using make.*
- `install` (line 172) `def install(self)` - *Run make install (requires sudo).*
- `start` (line 186) `def start(self)` - *Start the daemon (make run).*
- `stop` (line 196) `def stop(self)` - *Stop the daemon (make stop).*
- `status` (line 202) `def status(self)` - *Show status (make status).*
- `read_log` (line 207) `def read_log(self, lines)` - *Show log (make read).*
- `clean` (line 214) `def clean(self)` - *Clean build artifacts (make clean).*
- `full_chain` (line 220) `def full_chain(self)` - *Run setup, build, install.*

### SH (2 files)

#### `configure.sh`
**Path:** `configure.sh`

**Functions:**
- `msg_info` (line 53) - *---------------------------------------------------------------------------- Helper functions -----------------------------------------------------...*
- `msg_ok` (line 54)
- `msg_warn` (line 55)
- `msg_error` (line 56)
- `command_exists` (line 59) - *Check if a command exists*
- `check_header` (line 64) - *Check if a C header exists by trying to compile a tiny program*
- `can_read_device` (line 71) - *Check if we can read a device file (by opening it)*

#### `install.sh`
**Path:** `install.sh`

**Functions:**
- `info` (line 25) - *---------------------------------------------------------------------------- Helper functions -----------------------------------------------------...*
- `ok` (line 26)
- `warn` (line 27)
- `error` (line 28)
- `check_root` (line 31) - *Check if we are running as root (or with sudo)*
- `install_packages` (line 38) - *Detect the package manager and install packages*
- `main` (line 67) - *---------------------------------------------------------------------------- Main installation routine --------------------------------------------...*
