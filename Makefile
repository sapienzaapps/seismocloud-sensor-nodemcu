
.PHONY: console
console:
	minicom -o -D ${PORT} -b 115200
