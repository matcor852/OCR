.SILENT: all
.SILENT: run
.SILENT: clean
.SILENT: save
.SILENT: format

all:
	clang -Wall -Wextra -std=c99 -O3 \
	-MMD *.c -c -g `pkg-config --cflags gtk+-3.0`
	clang *.o -o main `pkg-config --libs gtk+-3.0`

run:
	make
	clear
	echo "==================== Running main ====================="
	./main -t Images/image_03.jpeg
	echo "==================== End of output ===================="
	rm main *.d

clean:
	rm -rf *.[do] main *.out
	rm -rf COMMIT_MESSAGE
	clear

save:
	make clean
	echo "Enter commit message (optional):"
	read var; \
	echo $$var > COMMIT_MESSAGE
	git add --all
	git status --porcelain >> COMMIT_MESSAGE
	git commit -F COMMIT_MESSAGE
	
format:
	clang-format --style=file -i *.[ch]