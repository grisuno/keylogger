# Concepts

Second-brain semantic layer: nouns map atomically to file sets (EXTRACTED); verbs aggregate structural edges (INFERRED).

| Concept | Files | Mentions | Top Files |
|---------|-------|----------|-----------|
| `keylogger` | 4 | 13 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c` |
| `check` | 4 | 10 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c` |
| `user` | 4 | 4 | `app.py`, `configure.sh`, `install.sh`, `keylogger.c` |
| `make` | 3 | 18 | `app.py`, `configure.sh`, `install.sh` |
| `install` | 3 | 17 | `app.py`, `configure.sh`, `install.sh` |
| `file` | 3 | 13 | `app.py`, `configure.sh`, `keylogger.c` |
| `run` | 3 | 12 | `app.py`, `configure.sh`, `keylogger.c` |
| `input` | 3 | 11 | `configure.sh`, `install.sh`, `keylogger.c` |
| `event` | 3 | 10 | `configure.sh`, `install.sh`, `keylogger.c` |
| `linux` | 3 | 7 | `app.py`, `configure.sh`, `install.sh` |
| `log` | 3 | 7 | `app.py`, `configure.sh`, `keylogger.c` |
| `can` | 3 | 6 | `app.py`, `configure.sh`, `install.sh` |
| `dev` | 3 | 5 | `configure.sh`, `install.sh`, `keylogger.c` |
| `script` | 3 | 5 | `app.py`, `configure.sh`, `install.sh` |
| `sudo` | 3 | 5 | `app.py`, `configure.sh`, `install.sh` |
| `usage` | 3 | 4 | `app.py`, `configure.sh`, `keylogger.c` |
| `error` | 3 | 3 | `app.py`, `configure.sh`, `install.sh` |
| `evdev` | 3 | 3 | `app.py`, `configure.sh`, `install.sh` |
| `info` | 3 | 3 | `app.py`, `configure.sh`, `install.sh` |
| `project` | 3 | 3 | `app.py`, `configure.sh`, `install.sh` |
| `warn` | 3 | 3 | `app.py`, `configure.sh`, `install.sh` |
| `read` | 2 | 8 | `app.py`, `configure.sh` |
| `build` | 2 | 7 | `app.py`, `install.sh` |
| `config` | 2 | 7 | `configure.sh`, `keylogger.c` |
| `configure` | 2 | 7 | `app.py`, `configure.sh` |
| `device` | 2 | 7 | `configure.sh`, `keylogger.c` |
| `command` | 2 | 6 | `app.py`, `configure.sh` |
| `exists` | 2 | 6 | `app.py`, `configure.sh` |
| `all` | 2 | 5 | `app.py`, `keylogger.c` |
| `based` | 2 | 4 | `app.py`, `configure.sh` |
| `configuration` | 2 | 4 | `configure.sh`, `keylogger.c` |
| `installation` | 2 | 4 | `app.py`, `install.sh` |
| `root` | 2 | 4 | `configure.sh`, `install.sh` |
| `compile` | 2 | 3 | `app.py`, `configure.sh` |
| `group` | 2 | 3 | `configure.sh`, `install.sh` |
| `help` | 2 | 3 | `app.py`, `configure.sh` |
| `parse` | 2 | 3 | `app.py`, `keylogger.c` |
| `via` | 2 | 3 | `app.py`, `keylogger.c` |
| `current` | 2 | 2 | `configure.sh`, `install.sh` |
| `default` | 2 | 2 | `app.py`, `keylogger.c` |
| `environment` | 2 | 2 | `app.py`, `configure.sh` |
| `exist` | 2 | 2 | `app.py`, `install.sh` |
| `functions` | 2 | 2 | `configure.sh`, `install.sh` |
| `gcc` | 2 | 2 | `configure.sh`, `install.sh` |
| `headers` | 2 | 2 | `configure.sh`, `install.sh` |
| `helper` | 2 | 2 | `configure.sh`, `install.sh` |
| `loop` | 2 | 2 | `app.py`, `keylogger.c` |
| `output` | 2 | 2 | `app.py`, `keylogger.c` |
| `prerequisite` | 2 | 2 | `app.py`, `install.sh` |
| `return` | 2 | 2 | `app.py`, `keylogger.c` |

## Dialectic Prompts

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
