# zst

## Brief
`zst.h` is a single-header C library, which means `zero start`. 

Simply copy this file into your project and define a few macros to enable the corresponding functions.

However, keep in mind that this library is not focused on performance. It's designed for quick development of features. 

I haven’t spent too much time on error handling, so sometimes it’s just basic assertions. 

When using it, it's important to know what you're doing and make sure the parameters are valid.

## Usage

```console
$ cd examples
$ gcc -I ../ build.c
$ ./a.out --help
$ ./a.out --compile
$ ./log
[ERROR] This is error message
[INFO] This is info message
[GOOD] This is ok message
$ ./a.out --clean
```

For more usage examples, you can refer to the examples and the source code.

## Note

Since `zst.h` implements a generic data structure, the elements you push into it must be pointers.

For example, if you want to push an integer, you should pass an `int *`.

Because strings are not of fixed length, they cannot be directly used as elements in these generic data structures.

Therefore, we use `char **` as the element type for the data structure, which points to the string.

Since what we actually intend to push into the data structure is `char **`, we need to pass `char ***`.

If you find this approach too complex, you can treat `struct zd_string` as just a wrapper around `char *`. 

You don't have to use the full `struct zd_string` API; you can use it just like a `char *` without any difference.
