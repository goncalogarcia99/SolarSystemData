# SolarSystemData
A program for querying data about the solar system, obtained from [The Solar System OpenData](https://api.le-systeme-solaire.net/en/).

## Installation

### Dependencies (Ubuntu)
```shell script
$ sudo apt install libcurl14-gnutls-dev
$ sudo apt install libjansson-dev
$ sudo apt install valgrind
```

### Running Instructions
```shell script
$ make
$ export LD_LIBRARY_PATH=.
$ valgrind --leak-check=full ./solar https://api.le-systeme-solaire.net/rest/bodies
```

**Note**  
valgrind --leak-check=full can be omitted, this way a memory check won't be performed.

