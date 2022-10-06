.SILENT: all
.SILENT: run
.SILENT: clean

all:
	gcc -Wall -Wextra -std=c99 -O3 -MMD *.c -c
	gcc *.o -o main -lm -lSDL2 -lSDL2_image
run:
	make
	clear
	echo "========== Running main =========="
	./main
	echo "========== End of output =========="
	rm main *.d


clean:
	rm -rf *[.do] main test* edited.png
	clear
