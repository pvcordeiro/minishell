# 🐚 Minishell 🐚

## 📝 Description

Minishell is a simplified shell implementation that mimics the basic behavior of bash. This project is about understanding the inner workings of shells, process creation, and command interpretation in Unix systems.

## ✨ Features

- 🔍 Command interpretation with a beautiful prompt
- 🔄 Support for command history
- 🔀 Redirections (`<`, `>`, `<<`, `>>`)
- 📊 Pipes (`|`)
- 🌿 Environment variables (with `$` expansion)
- 💼 Built-in commands:
	- `echo`
	- `cd`
	- `pwd`
	- `export`
	- `unset`
	- `env`
	- `exit`
- 📢 Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)
- 🚫 Error handling with appropriate error messages

## 🛠️ Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git

# Change to project directory
cd minishell

# Compile
make

# Run
./minishell
```

## 🚀 Usage

Once launched, you'll see a prompt waiting for your commands:

```bash
🐚minishell🐚 ls -la
🐚minishell🐚 cat file.txt | grep "pattern" > output.txt
🐚minishell🐚 echo $HOME
```

## 📚 Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👥 Contributors

- afpachec(https://github.com/afonsopc)

Happy Shell Coding! 🎉