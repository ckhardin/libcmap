# libcmap

This library is a "c map" library that started as a basic
utilty map that was intended to provide the similiar hash and
tree map from Java.

Over the course of time, the problem set has evolved for this
to be a C11 based library that can combine a few things from
toml, json, and maps that were written to handle basically
a configuration file parse (and write).

## QuickStart

The build is cmake based and setup to create a based library
that can be included into other projects as needed.

```
$ cmake -S . -B build -Gninja
$ cmake --build build
$ ninja -C build test
```

## Contributing

This repo is using conventional commits for the messages and
has basic hooks for doing ci workflows to verify some basic
functional areas.
