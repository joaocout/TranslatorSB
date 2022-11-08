tradutor:
	g++ -std=c++11 -o tradutor tradutor.cpp

test:
	nasm -felf -o test.o test.s
	ld -m elf_i386 -o test test.o
	./test
	rm test.o test 
