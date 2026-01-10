# CppShell: A C++ Shell Command Line

This is a small experiment to write a shell and its commands from scratch using C++.

## How to use CppShell

Clone this repo and run `make shell` inside. It will compile everything and launch the shell. Traditional zsh/bash commands will also work in the cpp if you have them in your default bin in `$PATH` (chances are you do). However, the goal is to rewrite how these commands work from scratch. Implemented so far:

| command | description      | zsh/bash equivalent |
|---------|------------------|---------------------|
| cd      | change directory | cd                  |
| exit    | exit shell       | exit                |
| cwd    | print current working directory       | pwd                |
| new    | create new file/directory      | touch/mkdir               |
| delete | delete file/directory | rm |
| move   | move/rename a file | mv | 
|list    | list file information in current directory | ls |
