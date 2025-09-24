# minishell
A custom Unix shell implementation with advanced parsing and execution features, developed as part of the 42 School curriculum.

## Overview
This project implements a fully functional Unix shell that replicates the behavior of bash. It features a complete lexer and parser for command-line input, supports complex command structures including pipes and logical operators, implements built-in commands, and handles advanced features like wildcards, redirections, and subshells with parentheses.

## Features
### Core Shell Functionality
- Interactive command prompt with readline integration
- Command history navigation and persistence
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)
- Environment variable management and expansion
- Exit status tracking and `$?` variable support

### Built-in Commands
- **echo** [-n] [args...] - Display text with optional newline suppression
- **cd** [path] - Change directory with HOME and OLDPWD support
- **pwd** - Print working directory
- **export** [var[=value]...] - Set environment variables
- **unset** [var...] - Remove environment variables
- **env** - Display environment variables
- **exit** [code] - Exit shell with optional exit code

### Advanced Parsing
- **Lexical Analysis**: Complete tokenizer with quote handling
- **Syntax Validation**: Comprehensive error checking for malformed input
- **AST Construction**: Abstract syntax tree for command execution
- **Quote Processing**: Single and double quote parsing with variable expansion
- **Wildcard Expansion**: Glob pattern matching with `*` support

### Execution Engine
- **Pipeline Support**: Multi-command pipes with proper file descriptor management
- **Logical Operators**: `&&` (AND) and `||` (OR) command chaining
- **Parenthetical Grouping**: Subshell execution with `()` syntax
- **Redirections**: Input/output redirection (`<`, `>`, `>>`) and heredocs (`<<`)
- **Process Management**: Fork-based command execution with proper cleanup

## Compilation
### Prerequisites
- GCC or compatible C compiler
- Make build system
- Readline library (libreadline-dev)
- POSIX-compliant system (Linux/macOS)

### Build Commands
```bash
# Compile the shell
make

# Clean object files
make clean

# Full cleanup
make fclean

# Rebuild everything
make re
```

## Usage
### Basic Commands
```bash
# Start the shell
./minishell

# Execute simple commands
minishell$ ls -la
minishell$ echo "Hello, World!"
minishell$ pwd

# Change directories
minishell$ cd /tmp
minishell$ cd        # Go to HOME
```

### Pipes and Redirections
```bash
# Simple pipes
minishell$ ls | grep .c | wc -l

# Output redirection
minishell$ echo "test" > output.txt
minishell$ cat input.txt >> output.txt

# Input redirection
minishell$ wc -l < input.txt

# Here-documents
minishell$ cat << EOF
Hello
World
EOF
```

### Logical Operators
```bash
# AND operator (execute second if first succeeds)
minishell$ make && ./minishell

# OR operator (execute second if first fails)
minishell$ invalid_command || echo "Command failed"

# Complex chaining
minishell$ cd /tmp && ls -la || echo "Failed to list directory"
```

### Parenthetical Grouping
```bash
# Subshell execution
minishell$ (cd /tmp && ls) && pwd

# Complex grouping with pipes
minishell$ (echo "line1"; echo "line2") | wc -l

# Logical operators with groups
minishell$ (cd /nonexistent && ls) || echo "Directory change failed"
```

### Variable Operations
```bash
# Set and export variables
minishell$ export NAME="John Doe"
minishell$ export PATH="/usr/local/bin:$PATH"

# Display variables
minishell$ env | grep NAME
minishell$ echo $NAME

# Unset variables
minishell$ unset NAME

# Exit status
minishell$ echo $?
```

## Advanced Features
### Wildcard Expansion
```bash
# Match all .c files
minishell$ ls *.c

# Complex patterns
minishell$ echo src/*.c include/*.h
```

### Quote Handling
```bash
# Preserve spaces and special characters
minishell$ echo "Hello    World"
minishell$ echo 'Variable: $HOME'

# Mixed quoting
minishell$ echo "Home is $HOME, user is `whoami`"
```

### Here-documents with Expansion
```bash
# Variable expansion in heredocs
minishell$ cat << EOF
Current user: $USER
Home directory: $HOME
EOF

# Quoted delimiter prevents expansion
minishell$ cat << 'EOF'
Literal: $USER
EOF
```

## Implementation Details
### Parsing Architecture
- **Tokenization**: Multi-state lexer handling quotes, operators, and words
- **Validation**: Comprehensive syntax checking before execution
- **AST Construction**: Recursive descent parser building execution trees
- **Error Recovery**: Detailed error messages with proper exit codes

### Execution Model
- **Process Management**: Fork-based execution with proper signal handling
- **File Descriptor Management**: Safe redirection with cleanup
- **Pipeline Implementation**: Multi-process pipes with synchronization
- **Built-in Optimization**: Parent-process execution for shell built-ins

### Memory Management
- **Dynamic Allocation**: Efficient token and AST node management
- **Cleanup Strategies**: Proper resource deallocation on exit
- **Error Handling**: Graceful recovery from allocation failures

## Error Handling
```bash
# Syntax errors
minishell$ echo "unclosed quote
bash: unexpected EOF while looking for matching `"'

# Command not found
minishell$ nonexistent_command
minishell: nonexistent_command: command not found

# Permission errors
minishell$ cd /root
minishell: cd: /root: Permission denied
```

## Signal Handling
- **SIGINT (Ctrl+C)**: Interrupt current command, display new prompt
- **SIGQUIT (Ctrl+\\)**: Ignored in interactive mode
- **SIGTERM**: Graceful shutdown with cleanup
- **EOF (Ctrl+D)**: Exit shell cleanly

## Standards Compliance
- **POSIX Shell**: Compatible with standard shell behavior
- **42 Norm**: Follows strict coding standards (Norminette compliant)
- **Memory Safety**: Zero memory leaks with proper resource management
- **Signal Safety**: Proper signal handling in multi-process environment

## Authors
**yyudi & nweber** - 42 Heilbronn Students
