all:
	gcc `find . -type f -iname "*.c"` -I src -I src/schrift -L src/schrift -lschrift -o consdraw `pkg-config --cflags --libs libpng`