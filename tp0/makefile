all: sh sh.c

run: sh
	clear
	bin/sh.out

sh: src/sh.c bin
	gcc -Wall -Wno-infinite-recursion src/sh.c -o sh
	cp sh bin/sh.out

sh.c:
	ln -sf src/sh.c sh.c

mark:
	clear
	python3 ./tests/getmarks.py < src/sh.c

test: sh sh.c
	clear
	./tests/grade.sh
	rm -f sh

help:
	@echo "COMMANDS:\nrun\nmark\ntest\nhelp"

clean:
	rm -f sh sh.c bin/sh.out