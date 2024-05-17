src := $(wildcard src/*.c)
src := $(filter-out src/test.c, $(src))
test_src := src/test.c src/mystring.c src/fs.c

shitty_build: src
	clang $(src) -o araw

test:
	clang $(test_src) -o test
	@./test
	@rm test
