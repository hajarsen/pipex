*This project has been created as part of the 42 curriculum by hsennane.*

# Pipex

## Description

**Pipex** is a C project from the **42 / 1337 curriculum** that explores how Unix shells execute commands and connect them with pipes.

The goal is to reproduce the basic behavior of the following shell pipeline:

```bash
< infile cmd1 | cmd2 > outfile
```

Instead of relying on the shell to connect the commands, the program works directly with **processes**, **file descriptors**, **redirections**, and **system calls**.

## Instructions

### Building the Project

Requirements: a Unix-like system (Linux is recommended), `cc`, and `make`.

```bash
git clone https://github.com/hajarsen/pipex.git
cd pipex
make
```

The Makefile builds the executable **`pipex`** with `-Wall -Wextra -Werror`.

### Running the Project

The current program expects four arguments after the executable:

```bash
./pipex infile "cmd1" "cmd2" outfile
```

| Argument | Purpose |
|---|---|
| `infile` | File used as the first command's standard input |
| `cmd1` | First command |
| `cmd2` | Second command |
| `outfile` | File receiving the final output |

### Example 1: Filter and Count Lines

```bash
printf 'apple\nbanana\napple pie\n' > infile
./pipex infile "grep apple" "wc -l" outfile
cat outfile
```

Intended output:

```text
2
```

Shell equivalent:

```bash
< infile grep apple | wc -l > outfile
```

### Example 2: Count Words

```bash
printf 'hello world\npipex project\n' > infile
./pipex infile "cat" "wc -w" outfile
cat outfile
```

Intended output:

```text
4
```

### Makefile Commands

| Command | Action |
|---|---|
| `make` / `make all` | Build the executable |
| `make clean` | Remove object files |
| `make fclean` | Remove objects and executable |
| `make re` | Rebuild everything |

### Project Files

```text
pipex/
├── main.c                  # Entry point and argument checks
├── pipex.h                 # Structures and declarations
├── init_pipex.c            # Initialize pipeline state
├── pipex_command_parser.c  # Split command arguments and find paths
├── pipex_execution.c       # Create pipes and child processes
├── io_and_proc.c           # Input/output redirection and execve
├── pipex_cleanup.c         # Free allocated resources
├── err_utils.c             # Error handling
├── utils1.c
├── utils2.c
├── utils3.c                # Utility functions
├── Makefile
└── README.md
```

### Evaluation Notes

- The current `main.c` requires **exactly two commands** (`argc == 5`).
- Commands are split on spaces; the parser does **not** implement full shell quoting or expansion.
- The output file is created if needed and otherwise truncated (`O_TRUNC`).
- Features such as `here_doc`, append mode, and more than two commands are not handled by this entry point.
- This repository does not contain an automated test suite, so the examples above describe intended behavior, not verified test results.

## Resources

Understanding the following Unix concepts makes the implementation easier to follow.

---

## 🔧 File Descriptors

A **file descriptor** is a small integer that identifies an open file or an input/output stream for a process.

| Descriptor | Name | Default destination |
|---|---|---|
| `0` | Standard input (`stdin`) | Keyboard / terminal |
| `1` | Standard output (`stdout`) | Terminal |
| `2` | Standard error (`stderr`) | Terminal |

### Redirection with dup2()

`dup2(oldfd, newfd)` makes a chosen descriptor refer to the same open file description as another one.

```text
Before redirection:

   stdin (0)   ───► terminal input
   stdout (1)  ───► terminal display

After dup2(input_fd, 0):

   stdin (0)   ───► infile
   stdout (1)  ───► terminal display

After dup2(output_fd, 1):

   stdin (0)   ───► infile
   stdout (1)  ───► outfile
```

This is how a child process can read a file as though it were typing into a terminal.

---

## 🔗 Pipes

A **pipe** creates a one-way channel for transferring bytes between file descriptors.

```text
             pipe(fd)
            ┌──────────┐
   Writer ──►│  buffer  │──► Reader
             └──────────┘
                │    │
              fd[1] fd[0]
             write   read
```

### A Two-Command Pipeline

```text
     ┌────────────┐
     │   infile   │
     └──────┬─────┘
            │ stdin
            ▼
     ┌────────────┐          ┌────────────┐
     │    cmd1    │─ pipe ──►│    cmd2    │
     └────────────┘          └──────┬─────┘
                                    │ stdout
                                    ▼
                             ┌────────────┐
                             │  outfile   │
                             └────────────┘
```

The first command's output becomes the second command's input.

---

## 👥 fork() and Child Processes

`fork()` creates a new process. The parent and child continue executing from the call, but they receive different return values.

```text
                 Parent process
                       │
                     fork()
                       │
              ┌────────┴────────┐
              ▼                 ▼
         Parent branch      Child branch
         returns child PID  returns 0
              │                 │
         may launch next    configure stdin
         child process      and stdout
              │                 │
         waitpid(...)       execve(...)
              │                 │
         collect status     run command
```

The parent uses `waitpid()` to collect child-process completion status and avoid leaving zombie processes.

---

## 🚀 execve() and PATH

`execve()` replaces the current process image with a new executable program. A command name such as `grep` is not necessarily a full executable path.

The parser uses the `PATH` environment variable to look for an executable.

```text
Command: grep apple
        │
        ▼
Split: ["grep", "apple"]
        │
        ▼
Read PATH entries
        │
        ├── /usr/local/bin/grep
        ├── /usr/bin/grep       ◄── possible match
        └── /bin/grep
        │
        ▼
execve("/usr/bin/grep", args, envp)
```

Actual paths depend on the operating system. `execve` also receives the command's argument array and environment.

---

## 🧭 Full Pipeline Overview

```text
./pipex infile "grep apple" "wc -l" outfile
                     │
                     ▼
             Validate arguments
                     │
                     ▼
              Open input/output
                     │
                     ▼
             Parse command strings
                     │
                     ▼
            Resolve command paths
                     │
                     ▼
              Create pipe(s)
                     │
                     ▼
             Fork child processes
                     │
             ┌───────┴────────┐
             ▼                ▼
          Child 1          Child 2
         read infile      read pipe
         write pipe       write outfile
             │                │
             ▼                ▼
          execve()         execve()
             └────────┬───────┘
                      ▼
                Parent waits
                      ▼
                Exit status
```

---

## 🎯 Pipex Tips

1. Think about **which process owns each file descriptor**.
2. Close pipe ends that a process no longer needs; otherwise, readers may never see end-of-file.
3. Distinguish an executable's absolute path from a command found through `PATH`.
4. Compare intended output against the equivalent shell pipeline.
5. Check error handling for missing input files, unknown commands, and permission failures.

### Useful Links

- [pipe(2) — Linux manual](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [fork(2) — Linux manual](https://man7.org/linux/man-pages/man2/fork.2.html)
- [dup2(2) — Linux manual](https://man7.org/linux/man-pages/man2/dup.2.html)
- [execve(2) — Linux manual](https://man7.org/linux/man-pages/man2/execve.2.html)
- [waitpid(2) — Linux manual](https://man7.org/linux/man-pages/man2/waitpid.2.html)

### AI Usage

AI assistance was used to **structure the README, explain system calls, and create educational diagrams**. No C source files were modified in this documentation update.
