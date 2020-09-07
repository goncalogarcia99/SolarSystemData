A program for querying data about the solar system, obtained through the Solar System OpenData API.

Dependencies:
- sudo apt-get install libcurl14-gnutls-dev
- sudo apt-get install libjansson-dev
- sudo apt-get install valgrind

Running instructions:
- make
- export LD_LIBRARY_PATH=.
- valgrind --leak-check=full ./solar https://api.le-systeme-solaire.net/rest/bodies
- (valgrind --leak-check=full can be omitted, this way a memory check won't be performed)
