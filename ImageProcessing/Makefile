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
	clear

save:
	git add --all
	git commit -m "\"$(git status --porcelain)\""
	git push