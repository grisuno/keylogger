# API

## app.py

### info `def info(msg)`
- Defined: `app.py:53`

### ok `def ok(msg)`
- Defined: `app.py:54`

### warn `def warn(msg)`
- Defined: `app.py:55`

### error `def error(msg)`
- Defined: `app.py:56`

### run_command `def run_command(cmd, cwd, check, capture)`
- Defined: `app.py:61`
- Doc: Run a shell command and return its output/status.

### file_exists `def file_exists(path)`
- Defined: `app.py:81`

### dir_exists `def dir_exists(path)`
- Defined: `app.py:84`

### require_file `def require_file(path, description)`
- Defined: `app.py:87`

### require_dir `def require_dir(path, description)`
- Defined: `app.py:92`

### check_sudo `def check_sudo()`
- Defined: `app.py:97`
- Doc: Check if sudo is available and the user can run it.

### interactive_menu `def interactive_menu(orch)`
- Defined: `app.py:231`
- Doc: Show a text-based menu and loop until exit.

### parse_args `def parse_args()`
- Defined: `app.py:278`

### main `def main()`
- Defined: `app.py:306`

### __init__ `def __init__(self, base_dir)`
- Defined: `app.py:113`

### check_prerequisites `def check_prerequisites(self)`
- Defined: `app.py:123`
- Doc: Verify that all expected files exist.

### setup `def setup(self)`
- Defined: `app.py:135`
- Doc: Run install.sh and configure.

### build `def build(self)`
- Defined: `app.py:159`
- Doc: Compile the keylogger using make.

### install `def install(self)`
- Defined: `app.py:172`
- Doc: Run make install (requires sudo).

### start `def start(self)`
- Defined: `app.py:186`
- Doc: Start the daemon (make run).

### stop `def stop(self)`
- Defined: `app.py:196`
- Doc: Stop the daemon (make stop).

### status `def status(self)`
- Defined: `app.py:202`
- Doc: Show status (make status).

### read_log `def read_log(self, lines)`
- Defined: `app.py:207`
- Doc: Show log (make read).

### clean `def clean(self)`
- Defined: `app.py:214`
- Doc: Clean build artifacts (make clean).

### full_chain `def full_chain(self)`
- Defined: `app.py:220`
- Doc: Run setup, build, install.

## configure.sh

### msg_info
- Defined: `configure.sh:53`
- Doc: ---------------------------------------------------------------------------- Helper functions --------------------------

### msg_ok
- Defined: `configure.sh:54`

### msg_warn
- Defined: `configure.sh:55`

### msg_error
- Defined: `configure.sh:56`

### command_exists
- Defined: `configure.sh:59`
- Doc: Check if a command exists

### check_header
- Defined: `configure.sh:64`
- Doc: Check if a C header exists by trying to compile a tiny program

### can_read_device
- Defined: `configure.sh:71`
- Doc: Check if we can read a device file (by opening it)

## install.sh

### info
- Defined: `install.sh:25`
- Doc: ---------------------------------------------------------------------------- Helper functions --------------------------

### ok
- Defined: `install.sh:26`

### warn
- Defined: `install.sh:27`

### error
- Defined: `install.sh:28`

### check_root
- Defined: `install.sh:31`
- Doc: Check if we are running as root (or with sudo)

### install_packages
- Defined: `install.sh:38`
- Doc: Detect the package manager and install packages

### main
- Defined: `install.sh:67`
- Doc: ---------------------------------------------------------------------------- Main installation routine -----------------

## keylogger.c

### signal_handler `static void signal_handler(int sig)`
- Defined: `keylogger.c:77`
- Doc: -------------------------------------------------------------------------- Signal handler ------------------------------

### shift_symbol `static const char *shift_symbol(unsigned int code)`
- Defined: `keylogger.c:168`
- Doc: -------------------------------------------------------------------------- Shift-symbol mapping for US QWERTY ----------

### update_modifiers `static void update_modifiers(unsigned int code, int value)`
- Defined: `keylogger.c:198`
- Doc: -------------------------------------------------------------------------- Modifier updates ----------------------------

### key_to_string `static int key_to_string(unsigned int code, int value, char *out, size_t out_size)`
- Defined: `keylogger.c:212`
- Doc: -------------------------------------------------------------------------- Key code to string conversion (thread-safe, u

### write_log `static void write_log(const char *str)`
- Defined: `keylogger.c:271`
- Doc: -------------------------------------------------------------------------- Write a string to the log file (and stderr if

### is_keyboard_device `static int is_keyboard_device(int fd)`
- Defined: `keylogger.c:298`
- Doc: -------------------------------------------------------------------------- Check if a device is a keyboard via ioctl ---

### open_keyboard_by_index `static int open_keyboard_by_index(int idx)`
- Defined: `keylogger.c:314`
- Doc: -------------------------------------------------------------------------- Try to open a keyboard by event device index 

### scan_keyboards `static int scan_keyboards(int *fds, int max_count)`
- Defined: `keylogger.c:329`
- Doc: -------------------------------------------------------------------------- Scan all /dev/input/event* and return keyboar

### close_keyboards `static void close_keyboards(int *fds, int count)`
- Defined: `keylogger.c:345`
- Doc: -------------------------------------------------------------------------- Close all keyboard file descriptors ---------

### parse_config_line `static void parse_config_line(const char *line)`
- Defined: `keylogger.c:354`
- Doc: -------------------------------------------------------------------------- Parse a single config line (key = value) ----

### load_config `static void load_config(const char *path)`
- Defined: `keylogger.c:405`
- Doc: -------------------------------------------------------------------------- Load configuration from a file --------------

### init_config `static void init_config()`
- Defined: `keylogger.c:420`
- Doc: -------------------------------------------------------------------------- Initialize configuration from all known sourc

### print_usage `static void print_usage(const char *prog)`
- Defined: `keylogger.c:433`
- Doc: -------------------------------------------------------------------------- Print usage ---------------------------------

### process_event `static void process_event(struct input_event *ev)`
- Defined: `keylogger.c:444`
- Doc: -------------------------------------------------------------------------- Process event from a keyboard device --------

### handle_inotify `static int handle_inotify(int inotify_fd, int *fds, int *count)`
- Defined: `keylogger.c:456`
- Doc: -------------------------------------------------------------------------- Handle inotify event (new/removed devices in 

### run_keylogger `static void run_keylogger()`
- Defined: `keylogger.c:477`
- Doc: -------------------------------------------------------------------------- Main keylogger loop -------------------------

### daemonize `static void daemonize()`
- Defined: `keylogger.c:589`
- Doc: -------------------------------------------------------------------------- Daemonize (fork, detach from terminal) ------

### main `int main(int argc, char *argv[])`
- Defined: `keylogger.c:625`
- Doc: -------------------------------------------------------------------------- Main ----------------------------------------
