.SILENT: all
.SILENT: run

all:
	clang -Wall -Wextra -std=c99 -O3 -MMD *.c -o main -lSDL2 -lSDL2_image
	rm -rf main.d

run:
	make
	clear
	echo "========== Running main =========="
	./main
	echo "========== End of output =========="
	rm main
