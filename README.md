# SolarSystemData

A program for querying data about the solar system, obtained from [The Solar System OpenData](https://api.le-systeme-solaire.net/en/).

## Dependencies

### libcurl - URL Data Transfer

#### Installation

```shell
$ sudo apt-get install libcurl14-gnutls-dev
```

### libjansson - JSON Data Manipulation

#### Installation

```shell
$ sudo apt-get install libjansson-dev
```

## Installation
```shell
$ make
$ export LD_LIBRARY_PATH=.
```

## Running Instructions
```shell
$ ./solar https://api.le-systeme-solaire.net/rest/bodies/
```

## Optional Tools

### Valgrind - Memory Checker (Linux Only)

#### Installation

```shell
$ sudo apt-get install valgrind
```

#### Running Instructions

```shell
$ valgrind --leak-check=full ./program <args>
```

Valgrind's `--undef-value-errors=no` option disables warnings regarding undefined values (also speeds up the memory checking process), however, suppressing warnings isn't a good practice.

