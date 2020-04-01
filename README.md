# libcmap

This library is a "config map" library that started as a basic
utilty map that was intended to provide a Java hash and tree map
functionality.

Over the course of time, the problem set has evolved for this
to be a C11 based library that can combine a few things from
plists, json, and maps that were written to handle basically
a configuration file parse (and write).

## Build

The build is autotools based, so if this is checked out from source
code then the following steps should be performed.

```
$ ./bootstrap
$ mkdir build
$ cd build
$ ../configure
$ make
$ make check
```
