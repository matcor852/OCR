#!/bin/sh
.SILENT: all
.SILENT: run
.SILENT: clean

all:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 -MMD *.c -c -g
	clang -fsanitize=address *.o -o main -lm -lSDL2 -lSDL2_image

run:
	make
	clear
	echo "========== Running main =========="
	./main
	echo "========== End of output =========="
	rm main *.d

clean:
	rm -rf *[.do] main
	rm -f rotated.png
	clear

save:
	echo "Enter commit message (optional):"; \
	read var; \
	echo $$var > COMMIT_MESSAGE
	git add --all
	git status --porcelain >> COMMIT_MESSAGE
	git commit -F COMMIT_MESSAGE

