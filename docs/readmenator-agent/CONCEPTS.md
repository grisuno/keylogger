# Concepts

Nouns map atomically to file sets (EXTRACTED); verbs aggregate structural edges (INFERRED).

- `keylogger` | files=4 | mentions=13 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c`
- `check` | files=4 | mentions=10 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c`
- `user` | files=4 | mentions=4 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c`
- `make` | files=3 | mentions=18 | `app.py`, `configure.sh`, `install.sh`
- `install` | files=3 | mentions=17 | `app.py`, `configure.sh`, `install.sh`
- `file` | files=3 | mentions=13 | `app.py`, `configure.sh`, `keylogger.c`
- `run` | files=3 | mentions=12 | `app.py`, `configure.sh`, `keylogger.c`
- `input` | files=3 | mentions=11 | `configure.sh`, `install.sh`, `keylogger.c`
- `event` | files=3 | mentions=10 | `configure.sh`, `install.sh`, `keylogger.c`
- `linux` | files=3 | mentions=7 | `app.py`, `configure.sh`, `install.sh`
- `log` | files=3 | mentions=7 | `app.py`, `configure.sh`, `keylogger.c`
- `can` | files=3 | mentions=6 | `app.py`, `configure.sh`, `install.sh`
- `dev` | files=3 | mentions=5 | `configure.sh`, `install.sh`, `keylogger.c`
- `script` | files=3 | mentions=5 | `app.py`, `configure.sh`, `install.sh`
- `sudo` | files=3 | mentions=5 | `app.py`, `configure.sh`, `install.sh`
- `usage` | files=3 | mentions=4 | `app.py`, `configure.sh`, `keylogger.c`
- `error` | files=3 | mentions=3 | `app.py`, `configure.sh`, `install.sh`
- `evdev` | files=3 | mentions=3 | `app.py`, `configure.sh`, `install.sh`
- `info` | files=3 | mentions=3 | `app.py`, `configure.sh`, `install.sh`
- `project` | files=3 | mentions=3 | `app.py`, `configure.sh`, `install.sh`
- `warn` | files=3 | mentions=3 | `app.py`, `configure.sh`, `install.sh`
- `read` | files=2 | mentions=8 | `app.py`, `configure.sh`
- `build` | files=2 | mentions=7 | `app.py`, `install.sh`
- `config` | files=2 | mentions=7 | `configure.sh`, `keylogger.c`
- `configure` | files=2 | mentions=7 | `app.py`, `configure.sh`
- `device` | files=2 | mentions=7 | `configure.sh`, `keylogger.c`
- `command` | files=2 | mentions=6 | `app.py`, `configure.sh`
- `exists` | files=2 | mentions=6 | `app.py`, `configure.sh`
- `all` | files=2 | mentions=5 | `app.py`, `keylogger.c`
- `based` | files=2 | mentions=4 | `app.py`, `configure.sh`
- `configuration` | files=2 | mentions=4 | `configure.sh`, `keylogger.c`
- `installation` | files=2 | mentions=4 | `app.py`, `install.sh`
- `root` | files=2 | mentions=4 | `configure.sh`, `install.sh`
- `compile` | files=2 | mentions=3 | `app.py`, `configure.sh`
- `group` | files=2 | mentions=3 | `configure.sh`, `install.sh`
- `help` | files=2 | mentions=3 | `app.py`, `configure.sh`
- `parse` | files=2 | mentions=3 | `app.py`, `keylogger.c`
- `via` | files=2 | mentions=3 | `app.py`, `keylogger.c`
- `current` | files=2 | mentions=2 | `configure.sh`, `install.sh`
- `default` | files=2 | mentions=2 | `app.py`, `keylogger.c`
- `environment` | files=2 | mentions=2 | `app.py`, `configure.sh`
- `exist` | files=2 | mentions=2 | `app.py`, `install.sh`
- `functions` | files=2 | mentions=2 | `configure.sh`, `install.sh`
- `gcc` | files=2 | mentions=2 | `configure.sh`, `install.sh`
- `headers` | files=2 | mentions=2 | `configure.sh`, `install.sh`
- `helper` | files=2 | mentions=2 | `configure.sh`, `install.sh`
- `loop` | files=2 | mentions=2 | `app.py`, `keylogger.c`
- `output` | files=2 | mentions=2 | `app.py`, `keylogger.c`
- `prerequisite` | files=2 | mentions=2 | `app.py`, `install.sh`
- `return` | files=2 | mentions=2 | `app.py`, `keylogger.c`

## Dialectic

- Thesis: `all` centralizes 2 files; Antithesis: `check` pulls 4 files with 2 shared (Jaccard 0.50); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `default` pulls 2 files with 2 shared (Jaccard 1.00); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `file` pulls 3 files with 2 shared (Jaccard 0.67); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `keylogger` pulls 4 files with 2 shared (Jaccard 0.50); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `log` pulls 3 files with 2 shared (Jaccard 0.67); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `loop` pulls 2 files with 2 shared (Jaccard 1.00); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `output` pulls 2 files with 2 shared (Jaccard 1.00); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `parse` pulls 2 files with 2 shared (Jaccard 1.00); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `return` pulls 2 files with 2 shared (Jaccard 1.00); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
- Thesis: `all` centralizes 2 files; Antithesis: `run` pulls 3 files with 2 shared (Jaccard 0.67); Synthesis: should they merge, split by layer, or keep `bridges` explicit?
