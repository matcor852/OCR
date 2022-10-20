.SILENT: all
.SILENT: run
.SILENT: clean

all:
	clang -Wall -Wextra -std=c99 -O3 -MMD *.c -c
	clang *.o -o main -lm -lSDL2 -lSDL2_image

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
	git add --all
	git status --porcelain > COMMIT_MESSAGE
	git commit -F COMMIT_MESSAGE
	cat COMMIT_MESSAGE
