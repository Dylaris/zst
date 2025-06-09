# zst

## Brief

`zst.h` is a single-header C library for building C projects, inspired by the `tsoding/nobuild` project. 

The name "zst" stands for "zero start", meaning it’s designed for environments without build tools like Makefile. It gives you the feeling of starting from scratch, where all you need is a good C compiler to build a C project.

To use `zst`, you should be comfortable with C and writing "C scripts" (similar to shell scripts, but with more type checks and memory management).

`zst` works on both Windows and Linux, offering a unified API for easier project portability.

## Features

- **`zst_forger`**: A build tool, named "forger".

- **`zst_flag`**: A command-line flag parser.

- **`zst_fs`**: File system operations.

- **`zst_ds`**: Data structures, including +=dynamic array=+, +=string=+, +=stack=+, and +=queue=+.

## Usage

```console
$ cd examples/hello
$ gcc -o build build.c -I ../..
$ ./build
$ ./hello
hello world
```
