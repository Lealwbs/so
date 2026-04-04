all: sh
	
run: sh
	clear
	bin/sh.out

sh: src/sh.c bin
	gcc src/sh.c -o bin/sh.out

mark:
	clear
	python3 ./tests/getmarks.py < src/sh.c

test:
	clear
	./tests/grade.sh
	rm ./sh