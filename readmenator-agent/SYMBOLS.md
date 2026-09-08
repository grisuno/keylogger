# Symbols

| Symbol | Kind | File:Line | Signature |
|--------|------|-----------|-----------|
| `Colours` | class | `app.py:43` | `class Colours` |
| `Orchestrator` | class | `app.py:112` | `class Orchestrator` |
| `__init__` | method | `app.py:113` | `def __init__(self, base_dir)` |
| `build` | method | `app.py:159` | `def build(self)` |
| `check_prerequisites` | method | `app.py:123` | `def check_prerequisites(self)` |
| `check_sudo` | method | `app.py:97` | `def check_sudo()` |
| `clean` | method | `app.py:214` | `def clean(self)` |
| `dir_exists` | method | `app.py:84` | `def dir_exists(path)` |
| `error` | method | `app.py:56` | `def error(msg)` |
| `file_exists` | method | `app.py:81` | `def file_exists(path)` |
| `full_chain` | method | `app.py:220` | `def full_chain(self)` |
| `info` | method | `app.py:53` | `def info(msg)` |
| `install` | method | `app.py:172` | `def install(self)` |
| `interactive_menu` | method | `app.py:231` | `def interactive_menu(orch)` |
| `main` | method | `app.py:306` | `def main()` |
| `ok` | method | `app.py:54` | `def ok(msg)` |
| `parse_args` | method | `app.py:278` | `def parse_args()` |
| `read_log` | method | `app.py:207` | `def read_log(self, lines)` |
| `require_dir` | method | `app.py:92` | `def require_dir(path, description)` |
| `require_file` | method | `app.py:87` | `def require_file(path, description)` |
| `run_command` | method | `app.py:61` | `def run_command(cmd, cwd, check, capture)` |
| `setup` | method | `app.py:135` | `def setup(self)` |
| `start` | method | `app.py:186` | `def start(self)` |
| `status` | method | `app.py:202` | `def status(self)` |
| `stop` | method | `app.py:196` | `def stop(self)` |
| `warn` | method | `app.py:55` | `def warn(msg)` |
| `can_read_device` | function | `configure.sh:71` | `` |
| `check_header` | function | `configure.sh:64` | `` |
| `command_exists` | function | `configure.sh:59` | `` |
| `msg_error` | function | `configure.sh:56` | `` |
| `msg_info` | function | `configure.sh:53` | `` |
| `msg_ok` | function | `configure.sh:54` | `` |
| `msg_warn` | function | `configure.sh:55` | `` |
| `check_root` | function | `install.sh:31` | `` |
| `error` | function | `install.sh:28` | `` |
| `info` | function | `install.sh:25` | `` |
| `install_packages` | function | `install.sh:38` | `` |
| `main` | function | `install.sh:67` | `` |
| `ok` | function | `install.sh:26` | `` |
| `warn` | function | `install.sh:27` | `` |
| `CONF_FILE_SYSTEM` | macro | `keylogger.c:37` | `#define CONF_FILE_SYSTEM` |
| `CONF_FILE_USER` | macro | `keylogger.c:38` | `#define CONF_FILE_USER` |
| `CONF_LINE_MAX` | macro | `keylogger.c:42` | `#define CONF_LINE_MAX` |
| `EVENT_BUF_LEN` | macro | `keylogger.c:43` | `#define EVENT_BUF_LEN` |
| `INOTIFY_BUF_LEN` | macro | `keylogger.c:44` | `#define INOTIFY_BUF_LEN` |
| `KEY_MAP_SIZE` | macro | `keylogger.c:41` | `#define KEY_MAP_SIZE` |
| `LOG_FILE_DEFAULT` | macro | `keylogger.c:35` | `#define LOG_FILE_DEFAULT` |
| `MAX_KEYBOARDS` | macro | `keylogger.c:40` | `#define MAX_KEYBOARDS` |
| `PID_FILE` | macro | `keylogger.c:36` | `#define PID_FILE` |
| `POLL_TIMEOUT_MS` | macro | `keylogger.c:39` | `#define POLL_TIMEOUT_MS` |
| `_GNU_SOURCE` | macro | `keylogger.c:13` | `#define _GNU_SOURCE` |
| `chdir` | function | `keylogger.c:610` | `chdir("/");` |
| `clock_gettime` | function | `keylogger.c:275` | `clock_gettime(CLOCK_REALTIME, &ts);` |
| `close` | function | `keylogger.c:320` | `close(fd);` |
| `close_keyboards` | function | `keylogger.c:345` | `static void close_keyboards(int *fds, int count)` |
| `config_t` | struct | `keylogger.c:49` | `` |
| `daemonize` | function | `keylogger.c:589` | `static void daemonize()` |
| `dup` | function | `keylogger.c:618` | `dup(0);` |
| `exit` | function | `keylogger.c:593` | `exit(EXIT_FAILURE);` |
| `fclose` | function | `keylogger.c:286` | `fclose(fp);` |
| `fflush` | function | `keylogger.c:285` | `fflush(fp);` |
| `fprintf` | function | `keylogger.c:284` | `fprintf(fp, "[%s.%06ld] %s\n", timebuf, ts.tv_nsec / 1000, str);` |
| `free` | function | `keylogger.c:505` | `free(pfds);` |
| `handle_inotify` | function | `keylogger.c:456` | `static int handle_inotify(int inotify_fd, int *fds, int *count)` |
| `init_config` | function | `keylogger.c:420` | `static void init_config()` |
| `is_keyboard_device` | function | `keylogger.c:298` | `static int is_keyboard_device(int fd)` |
| `key_to_string` | function | `keylogger.c:212` | `static int key_to_string(unsigned int code, int value, char *out, size_t out_size)` |
| `load_config` | function | `keylogger.c:405` | `static void load_config(const char *path)` |
| `localtime_r` | function | `keylogger.c:277` | `localtime_r(&ts.tv_sec, &tm);` |
| `main` | function | `keylogger.c:625` | `int main(int argc, char *argv[])` |
| `memcpy` | function | `keylogger.c:243` | `memcpy(out, sym, len + 1);` |
| `open` | function | `keylogger.c:616` | `open("/dev/null", O_RDWR);` |
| `open_keyboard_by_index` | function | `keylogger.c:314` | `static int open_keyboard_by_index(int idx)` |
| `parse_config_line` | function | `keylogger.c:354` | `static void parse_config_line(const char *line)` |
| `perror` | function | `keylogger.c:592` | `perror("fork");` |
| `print_usage` | function | `keylogger.c:433` | `static void print_usage(const char *prog)` |
| `process_event` | function | `keylogger.c:444` | `static void process_event(struct input_event *ev)` |
| `run_keylogger` | function | `keylogger.c:477` | `static void run_keylogger()` |
| `scan_keyboards` | function | `keylogger.c:329` | `static int scan_keyboards(int *fds, int max_count)` |
| `shift_symbol` | function | `keylogger.c:168` | `static const char *shift_symbol(unsigned int code)` |
| `signal` | function | `keylogger.c:601` | `signal(SIGCHLD, SIG_IGN);` |
| `signal_handler` | function | `keylogger.c:77` | `static void signal_handler(int sig)` |
| `snprintf` | function | `keylogger.c:316` | `snprintf(path, sizeof(path), "/dev/input/event%d", idx);` |
| `strftime` | function | `keylogger.c:279` | `strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm);` |
| `unlink` | function | `keylogger.c:665` | `unlink(PID_FILE);` |
| `update_modifiers` | function | `keylogger.c:198` | `static void update_modifiers(unsigned int code, int value)` |
| `write_log` | function | `keylogger.c:271` | `static void write_log(const char *str)` |
