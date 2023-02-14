all:
	gcc `find . -type f -iname "*.c"` -I src -o consdraw