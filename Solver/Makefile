all:
	clang -Wall -Wextra -std=c99 -O3 -MMD *.c -c
	clang *.o -o main

run:
	make
	./main
	rm main *.d

clean: 
	rm -rf *.[do] main
	clear

	
