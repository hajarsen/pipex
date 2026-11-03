# Pipex

A C project from the **42 / 1337 curriculum** that recreates the basic behavior of a Unix shell pipeline.

The program takes an input file, runs two commands connected by a pipe, and writes the result to an output file. This project is a practical introduction to processes, file descriptors, redirections, and command execution on Unix-like systems.

## How it works

The general idea is equivalent to this shell command:

```bash
< infile cmd1 | cmd2 > outfile
```

Pipex runs it using:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

The program uses `pipe()`, `fork()`, `dup2()`, `execve()`, and `waitpid()` to create the pipeline and manage child processes.

## Installation

### Requirements

- A Unix-like environment (Linux is recommended)
- A C compiler (`cc`)
- `make`

Clone and build the project:

```bash
git clone https://github.com/hajarsen/pipex.git
cd pipex
make
```

This creates the executable `pipex`. The Makefile compiles with `-Wall -Wextra -Werror`.

## Usage

The program expects exactly four arguments after the executable:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

| Argument | Description |
| --- | --- |
| `infile` | Input file to read |
| `cmd1` | First command |
| `cmd2` | Second command |
| `outfile` | Output file (created or overwritten) |

### Example 1: Count lines containing a word

```bash
printf "apple\nbanana\napple pie\n" > infile
./pipex infile "grep apple" "wc -l" outfile
cat outfile
```

Expected output:

```text
2
```

Equivalent shell pipeline:

```bash
< infile grep apple | wc -l > outfile
```

### Example 2: Count words

```bash
printf "hello world\npipex project\n" > infile
./pipex infile "cat" "wc -w" outfile
cat outfile
```

Expected output:

```text
4
```

## Makefile commands

| Command | Description |
| --- | --- |
| `make` / `make all` | Build `pipex` |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executable |
| `make re` | Rebuild the project |

## Project structure

```text
pipex/
├── main.c                  # Entry point and argument validation
├── pipex.h                 # Structures and function declarations
├── init_pipex.c            # Initialize project state
├── pipex_command_parser.c  # Parse commands and resolve executable paths
├── pipex_execution.c       # Pipes, processes and waiting
├── io_and_proc.c           # File-descriptor redirection and execution
├── pipex_cleanup.c         # Cleanup
├── err_utils.c             # Error helpers
├── utils1.c                # Utility functions
├── utils2.c
├── utils3.c
├── Makefile
└── README.md
```

## What I learned

- Working with Unix processes using `fork()`
- Creating pipes and connecting their ends
- Redirecting standard input and output with `dup2()`
- Executing programs with `execve()` and finding them through `PATH`
- Handling errors, process exit statuses, and resources

## Current scope

- This version requires **exactly two commands** (`argc == 5`).
- Command arguments are split on spaces. This is **not a full shell parser**: shell-style quotes, expansions, and other advanced syntax are not supported.
- Output is opened with `O_TRUNC`, so an existing output file is overwritten.
- Bonus features such as `here_doc`, append mode, or pipelines with more than two commands are not included in the current entry point.
- Automated tests are not included in this repository.

## Author

**hsennane** — 42 / 1337 project.
