all:
	gcc `find . -type f -iname "*.c"` -Wall -I src -I src/schrift -I src/hashtable -L src/schrift -lschrift -lm -lpng -o consdraw 