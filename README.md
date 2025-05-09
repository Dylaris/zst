# zd

## Brief
`zd` is a single-header C library. Simply copy this file into your project and define a few macros to enable the corresponding functions.

However, keep in mind that this library is not focused on performance. It's designed for quick development of features. 

I haven’t spent too much time on error handling, so sometimes it’s just basic assertions. 

When using it, it's important to know what you're doing and make sure the parameters are valid.

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

For more usage examples, you can refer to the examples and the source code.
