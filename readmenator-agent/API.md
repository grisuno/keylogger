# API

## app.py
- `Colours.info` (method) `app.py:53` `def info(msg)`
- `Colours.ok` (method) `app.py:54` `def ok(msg)`
- `Colours.warn` (method) `app.py:55` `def warn(msg)`
- `Colours.error` (method) `app.py:56` `def error(msg)`
- `Colours.run_command` (method) `app.py:61` `def run_command(cmd, cwd, check, capture)` -- Run a shell command and return its output/status.
- `Colours.file_exists` (method) `app.py:81` `def file_exists(path)`
- `Colours.dir_exists` (method) `app.py:84` `def dir_exists(path)`
- `Colours.require_file` (method) `app.py:87` `def require_file(path, description)`
- `Colours.require_dir` (method) `app.py:92` `def require_dir(path, description)`
- `Colours.check_sudo` (method) `app.py:97` `def check_sudo()` -- Check if sudo is available and the user can run it.
- `Orchestrator.__init__` (method) `app.py:113` `def __init__(self, base_dir)`
- `Orchestrator.check_prerequisites` (method) `app.py:123` `def check_prerequisites(self)` -- Verify that all expected files exist.
- `Orchestrator.setup` (method) `app.py:135` `def setup(self)` -- Run install.sh and configure.
- `Orchestrator.build` (method) `app.py:159` `def build(self)` -- Compile the keylogger using make.
- `Orchestrator.install` (method) `app.py:172` `def install(self)` -- Run make install (requires sudo).
- `Orchestrator.start` (method) `app.py:186` `def start(self)` -- Start the daemon (make run).
- `Orchestrator.stop` (method) `app.py:196` `def stop(self)` -- Stop the daemon (make stop).
- `Orchestrator.status` (method) `app.py:202` `def status(self)` -- Show status (make status).
- `Orchestrator.read_log` (method) `app.py:207` `def read_log(self, lines)` -- Show log (make read).
- `Orchestrator.clean` (method) `app.py:214` `def clean(self)` -- Clean build artifacts (make clean).
- `Orchestrator.full_chain` (method) `app.py:220` `def full_chain(self)` -- Run setup, build, install.
- `Orchestrator.interactive_menu` (method) `app.py:231` `def interactive_menu(orch)` -- Show a text-based menu and loop until exit.
- `Orchestrator.parse_args` (method) `app.py:278` `def parse_args()`
- `Orchestrator.main` (method) `app.py:306` `def main()`

## configure.sh
- `msg_info` (function) `configure.sh:53`
- `msg_ok` (function) `configure.sh:54`
- `msg_warn` (function) `configure.sh:55`
- `msg_error` (function) `configure.sh:56`
- `command_exists` (function) `configure.sh:59` -- Check if a command exists
- `check_header` (function) `configure.sh:64` -- Check if a C header exists by trying to compile a tiny program
- `can_read_device` (function) `configure.sh:71` -- Check if we can read a device file (by opening it)

## install.sh
- `info` (function) `install.sh:25`
- `ok` (function) `install.sh:26`
- `warn` (function) `install.sh:27`
- `error` (function) `install.sh:28`
- `check_root` (function) `install.sh:31` -- Check if we are running as root (or with sudo)
- `install_packages` (function) `install.sh:38` -- Detect the package manager and install packages
- `main` (function) `install.sh:67`

## keylogger.c
- `signal_handler` (function) `keylogger.c:77` `static void signal_handler(int sig)`
- `shift_symbol` (function) `keylogger.c:168` `static const char *shift_symbol(unsigned int code)`
- `update_modifiers` (function) `keylogger.c:198` `static void update_modifiers(unsigned int code, int value)`
- `key_to_string` (function) `keylogger.c:212` `static int key_to_string(unsigned int code, int value, char *out, size_t out_size)`
- `write_log` (function) `keylogger.c:271` `static void write_log(const char *str)`
- `is_keyboard_device` (function) `keylogger.c:298` `static int is_keyboard_device(int fd)`
- `open_keyboard_by_index` (function) `keylogger.c:314` `static int open_keyboard_by_index(int idx)`
- `scan_keyboards` (function) `keylogger.c:329` `static int scan_keyboards(int *fds, int max_count)`
- `close_keyboards` (function) `keylogger.c:345` `static void close_keyboards(int *fds, int count)`
- `parse_config_line` (function) `keylogger.c:354` `static void parse_config_line(const char *line)`
- `load_config` (function) `keylogger.c:405` `static void load_config(const char *path)`
- `init_config` (function) `keylogger.c:420` `static void init_config()`
- `print_usage` (function) `keylogger.c:433` `static void print_usage(const char *prog)`
- `process_event` (function) `keylogger.c:444` `static void process_event(struct input_event *ev)`
- `handle_inotify` (function) `keylogger.c:456` `static int handle_inotify(int inotify_fd, int *fds, int *count)`
- `run_keylogger` (function) `keylogger.c:477` `static void run_keylogger()`
- `daemonize` (function) `keylogger.c:589` `static void daemonize()`
- `main` (function) `keylogger.c:625` `int main(int argc, char *argv[])`
