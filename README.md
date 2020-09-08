# SolarSystemData
A program for querying data about the solar system, obtained from [The Solar System OpenData](https://api.le-systeme-solaire.net/).

## Installation

### Dependencies (Ubuntu):
```shell script
$ sudo apt-get install libcurl14-gnutls-dev
$ sudo apt-get install libjansson-dev
$ sudo apt-get install valgrind`
```

### Running Instructions:
```shell script
$ make
$ export LD_LIBRARY_PATH=.
$ valgrind --leak-check=full ./solar https://api.le-systeme-solaire.net/rest/bodies
```

**Note:**  
valgrind --leak-check=full can be omitted, this way a memory check won't be performed.
