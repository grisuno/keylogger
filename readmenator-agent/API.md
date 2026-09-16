# API

## app.py

### info (method) `def info(msg)`
- Defined: `app.py:53`

### ok (method) `def ok(msg)`
- Defined: `app.py:54`

### warn (method) `def warn(msg)`
- Defined: `app.py:55`

### error (method) `def error(msg)`
- Defined: `app.py:56`

### run_command (method) `def run_command(cmd, cwd, check, capture)`
- Defined: `app.py:61`
- Doc: Run a shell command and return its output/status.

### file_exists (method) `def file_exists(path)`
- Defined: `app.py:81`

### dir_exists (method) `def dir_exists(path)`
- Defined: `app.py:84`

### require_file (method) `def require_file(path, description)`
- Defined: `app.py:87`

### require_dir (method) `def require_dir(path, description)`
- Defined: `app.py:92`

### check_sudo (method) `def check_sudo()`
- Defined: `app.py:97`
- Doc: Check if sudo is available and the user can run it.

### interactive_menu (method) `def interactive_menu(orch)`
- Defined: `app.py:231`
- Doc: Show a text-based menu and loop until exit.

### parse_args (method) `def parse_args()`
- Defined: `app.py:278`

### main (method) `def main()`
- Defined: `app.py:306`

### __init__ (method) `def __init__(self, base_dir)`
- Defined: `app.py:113`

### check_prerequisites (method) `def check_prerequisites(self)`
- Defined: `app.py:123`
- Doc: Verify that all expected files exist.

### setup (method) `def setup(self)`
- Defined: `app.py:135`
- Doc: Run install.sh and configure.

### build (method) `def build(self)`
- Defined: `app.py:159`
- Doc: Compile the keylogger using make.

### install (method) `def install(self)`
- Defined: `app.py:172`
- Doc: Run make install (requires sudo).

### start (method) `def start(self)`
- Defined: `app.py:186`
- Doc: Start the daemon (make run).

### stop (method) `def stop(self)`
- Defined: `app.py:196`
- Doc: Stop the daemon (make stop).

### status (method) `def status(self)`
- Defined: `app.py:202`
- Doc: Show status (make status).

### read_log (method) `def read_log(self, lines)`
- Defined: `app.py:207`
- Doc: Show log (make read).

### clean (method) `def clean(self)`
- Defined: `app.py:214`
- Doc: Clean build artifacts (make clean).

### full_chain (method) `def full_chain(self)`
- Defined: `app.py:220`
- Doc: Run setup, build, install.

## configure.sh

### msg_info (function)
- Defined: `configure.sh:53`
- Doc: ---------------------------------------------------------------------------- Helper functions --------------------------

### msg_ok (function)
- Defined: `configure.sh:54`

### msg_warn (function)
- Defined: `configure.sh:55`

### msg_error (function)
- Defined: `configure.sh:56`

### command_exists (function)
- Defined: `configure.sh:59`
- Doc: Check if a command exists

### check_header (function)
- Defined: `configure.sh:64`
- Doc: Check if a C header exists by trying to compile a tiny program

### can_read_device (function)
- Defined: `configure.sh:71`
- Doc: Check if we can read a device file (by opening it)

## install.sh

### info (function)
- Defined: `install.sh:25`
- Doc: ---------------------------------------------------------------------------- Helper functions --------------------------

### ok (function)
- Defined: `install.sh:26`

### warn (function)
- Defined: `install.sh:27`

### error (function)
- Defined: `install.sh:28`

### check_root (function)
- Defined: `install.sh:31`
- Doc: Check if we are running as root (or with sudo)

### install_packages (function)
- Defined: `install.sh:38`
- Doc: Detect the package manager and install packages

### main (function)
- Defined: `install.sh:67`
- Doc: ---------------------------------------------------------------------------- Main installation routine -----------------

## keylogger.c

### signal_handler (function) `static void signal_handler(int sig)`
- Defined: `keylogger.c:77`
- Doc: -------------------------------------------------------------------------- Signal handler ------------------------------

### shift_symbol (function) `static const char *shift_symbol(unsigned int code)`
- Defined: `keylogger.c:168`
- Doc: -------------------------------------------------------------------------- Shift-symbol mapping for US QWERTY ----------

### update_modifiers (function) `static void update_modifiers(unsigned int code, int value)`
- Defined: `keylogger.c:198`
- Doc: -------------------------------------------------------------------------- Modifier updates ----------------------------

### key_to_string (function) `static int key_to_string(unsigned int code, int value, char *out, size_t out_size)`
- Defined: `keylogger.c:212`
- Doc: -------------------------------------------------------------------------- Key code to string conversion (thread-safe, u

### write_log (function) `static void write_log(const char *str)`
- Defined: `keylogger.c:271`
- Doc: -------------------------------------------------------------------------- Write a string to the log file (and stderr if

### is_keyboard_device (function) `static int is_keyboard_device(int fd)`
- Defined: `keylogger.c:298`
- Doc: -------------------------------------------------------------------------- Check if a device is a keyboard via ioctl ---

### open_keyboard_by_index (function) `static int open_keyboard_by_index(int idx)`
- Defined: `keylogger.c:314`
- Doc: -------------------------------------------------------------------------- Try to open a keyboard by event device index 

### scan_keyboards (function) `static int scan_keyboards(int *fds, int max_count)`
- Defined: `keylogger.c:329`
- Doc: -------------------------------------------------------------------------- Scan all /dev/input/event* and return keyboar

### close_keyboards (function) `static void close_keyboards(int *fds, int count)`
- Defined: `keylogger.c:345`
- Doc: -------------------------------------------------------------------------- Close all keyboard file descriptors ---------

### parse_config_line (function) `static void parse_config_line(const char *line)`
- Defined: `keylogger.c:354`
- Doc: -------------------------------------------------------------------------- Parse a single config line (key = value) ----

### load_config (function) `static void load_config(const char *path)`
- Defined: `keylogger.c:405`
- Doc: -------------------------------------------------------------------------- Load configuration from a file --------------

### init_config (function) `static void init_config()`
- Defined: `keylogger.c:420`
- Doc: -------------------------------------------------------------------------- Initialize configuration from all known sourc

### print_usage (function) `static void print_usage(const char *prog)`
- Defined: `keylogger.c:433`
- Doc: -------------------------------------------------------------------------- Print usage ---------------------------------

### process_event (function) `static void process_event(struct input_event *ev)`
- Defined: `keylogger.c:444`
- Doc: -------------------------------------------------------------------------- Process event from a keyboard device --------

### handle_inotify (function) `static int handle_inotify(int inotify_fd, int *fds, int *count)`
- Defined: `keylogger.c:456`
- Doc: -------------------------------------------------------------------------- Handle inotify event (new/removed devices in 

### run_keylogger (function) `static void run_keylogger()`
- Defined: `keylogger.c:477`
- Doc: -------------------------------------------------------------------------- Main keylogger loop -------------------------

### daemonize (function) `static void daemonize()`
- Defined: `keylogger.c:589`
- Doc: -------------------------------------------------------------------------- Daemonize (fork, detach from terminal) ------

### main (function) `int main(int argc, char *argv[])`
- Defined: `keylogger.c:625`
- Doc: -------------------------------------------------------------------------- Main ----------------------------------------

### memcpy (function) `memcpy(out, sym, len + 1);`
- Defined: `keylogger.c:243`

### clock_gettime (function) `clock_gettime(CLOCK_REALTIME, &ts);`
- Defined: `keylogger.c:275`

### localtime_r (function) `localtime_r(&ts.tv_sec, &tm);`
- Defined: `keylogger.c:277`

### strftime (function) `strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm);`
- Defined: `keylogger.c:279`

### fprintf (function) `fprintf(fp, "[%s.%06ld] %s\n", timebuf, ts.tv_nsec / 1000, str);`
- Defined: `keylogger.c:284`

### fflush (function) `fflush(fp);`
- Defined: `keylogger.c:285`

### fclose (function) `fclose(fp);`
- Defined: `keylogger.c:286`

### snprintf (function) `snprintf(path, sizeof(path), "/dev/input/event%d", idx);`
- Defined: `keylogger.c:316`

### close (function) `close(fd);`
- Defined: `keylogger.c:320`

### free (function) `free(pfds);`
- Defined: `keylogger.c:505`

### perror (function) `perror("fork");`
- Defined: `keylogger.c:592`

### exit (function) `exit(EXIT_FAILURE);`
- Defined: `keylogger.c:593`

### signal (function) `signal(SIGCHLD, SIG_IGN);`
- Defined: `keylogger.c:601`

### chdir (function) `chdir("/");`
- Defined: `keylogger.c:610`

### open (function) `open("/dev/null", O_RDWR);`
- Defined: `keylogger.c:616`

### dup (function) `dup(0);`
- Defined: `keylogger.c:618`

### unlink (function) `unlink(PID_FILE);`
- Defined: `keylogger.c:665`
- Doc: Clean up PID file
