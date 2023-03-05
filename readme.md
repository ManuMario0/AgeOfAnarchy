# Synthesis

This project is part of A. Auclair and E. Mera's personnal work done for 2021 TIPE.

## Compiling

### On Linux/MacOS

```shell
mkdir build
cd build
cmake ..
make
```

### On Windows

You first need Mingw32 to compile.

```shell
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```