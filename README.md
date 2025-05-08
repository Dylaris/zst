# zd

## Brief
`zd` is a single-header C library. Simply copy this file into your project and define a few macros to enable the corresponding functions.

## Usage

Example of using `zd_log`:

```console
$ cd examples
$ lua build.lua
$ ./zd_log
[ERRO] This is error message
[INFO] This is info message
[GOOD] This is ok message
$ lua build.lua -clean
```
