# root

*Community 0 | 4 files | cohesion 1.00*

## Definition

This community groups 4 file(s) rooted at `root` with dominant language sh (cohesion 1.00). Central symbols: `CONF_FILE_SYSTEM`, `CONF_FILE_USER`, `CONF_LINE_MAX`, `Colours`, `EVENT_BUF_LEN`, `INOTIFY_BUF_LEN`, `KEY_MAP_SIZE`, `LOG_FILE_DEFAULT`. Core file: `keylogger.c` (30 symbols). Documented purpose: configure - Environment verification script for the Linux keylogger (evdev-based) project.  Usage: ./configure [--prefix=PREFIX] [--help]  Checks: - C compiler .

## Files

| File | Language | Layer | Symbols | Doc |
|------|----------|-------|---------|-----|
| `app.py` | py | utility | 26 | yes |
| `configure.sh` | sh | infrastructure | 7 | yes |
| `install.sh` | sh | utility | 7 | yes |
| `keylogger.c` | c | utility | 30 | no |

## Key Symbols

- `Colours` (class, `app.py:43`) `class Colours`
- `info` (method, `app.py:53`) `def info(msg)`
- `ok` (method, `app.py:54`) `def ok(msg)`
- `warn` (method, `app.py:55`) `def warn(msg)`
- `error` (method, `app.py:56`) `def error(msg)`
- `run_command` (method, `app.py:61`) `def run_command(cmd, cwd, check, capture)` - Run a shell command and return its output/status.
- `file_exists` (method, `app.py:81`) `def file_exists(path)`
- `dir_exists` (method, `app.py:84`) `def dir_exists(path)`
- `require_file` (method, `app.py:87`) `def require_file(path, description)`
- `require_dir` (method, `app.py:92`) `def require_dir(path, description)`
- `check_sudo` (method, `app.py:97`) `def check_sudo()` - Check if sudo is available and the user can run it.
- `Orchestrator` (class, `app.py:112`) `class Orchestrator`
- `__init__` (method, `app.py:113`) `def __init__(self, base_dir)`
- `check_prerequisites` (method, `app.py:123`) `def check_prerequisites(self)` - Verify that all expected files exist.
- `setup` (method, `app.py:135`) `def setup(self)` - Run install.sh and configure.
- `build` (method, `app.py:159`) `def build(self)` - Compile the keylogger using make.
- `install` (method, `app.py:172`) `def install(self)` - Run make install (requires sudo).
- `start` (method, `app.py:186`) `def start(self)` - Start the daemon (make run).
- `stop` (method, `app.py:196`) `def stop(self)` - Stop the daemon (make stop).
- `status` (method, `app.py:202`) `def status(self)` - Show status (make status).
- `read_log` (method, `app.py:207`) `def read_log(self, lines)` - Show log (make read).
- `clean` (method, `app.py:214`) `def clean(self)` - Clean build artifacts (make clean).
- `full_chain` (method, `app.py:220`) `def full_chain(self)` - Run setup, build, install.
- `interactive_menu` (method, `app.py:231`) `def interactive_menu(orch)` - Show a text-based menu and loop until exit.
- `parse_args` (method, `app.py:278`) `def parse_args()`
- `main` (method, `app.py:306`) `def main()`
- `msg_info` (function, `configure.sh:53`) - ---------------------------------------------------------------------------- Helper functions ------
- `msg_ok` (function, `configure.sh:54`)
- `msg_warn` (function, `configure.sh:55`)
- `msg_error` (function, `configure.sh:56`)

## Internal vs External Edges

- Internal resolved imports (EXTRACTED): 0
- Cross-boundary resolved imports (EXTRACTED): 0

## Connections

- No cross-community bridges recorded. This community is self-contained.

## Risks

- [taint high] `app.py` -> `app.py` via `subprocess` (0 hops)
- [taint medium] `keylogger.c` -> `keylogger.c` via `input` (0 hops)
- [taint medium] `keylogger.c` -> `keylogger.c` via `input` (0 hops)

## Open Questions

- Why do 1 file(s) lack file-level docs (e.g. `keylogger.c`)? What purpose do they serve?
- Is the dangerous import `subprocess` in `app.py` still required, or can it be isolated?
- What would break if the most connected file in root changed?
- Should root be split, given cohesion 1.00?

## Sources

- `app.py`
- `configure.sh`
- `install.sh`
- `keylogger.c`
