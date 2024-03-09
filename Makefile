obj_files := tempfiles/2048.o \
			 tempfiles/game.o \
			 tempfiles/get-input.o \
			 tempfiles/rendering.o \
			 tempfiles/menus.o \
			 tempfiles/playfield-prerendering.o \
			 tempfiles/menu-prerendering.o \
			 tempfiles/qw_utils.o \
			 tempfiles/scores.o

2048: tempfiles $(obj_files)
	gcc $(obj_files) -o 2048

tempfiles:
	mkdir tempfiles -p

tempfiles/2048.o: src/2048.c
	gcc -c $? -o $@

tempfiles/game.o: src/game.c
	gcc -c $? -o $@

tempfiles/get-input.o: src/get-input.c
	gcc -c $? -o $@

tempfiles/rendering.o: src/rendering.c
	gcc -c $? -o $@

tempfiles/menus.o: src/menus.c
	gcc -c $? -o $@

tempfiles/playfield-prerendering.o: src/playfield-prerendering.c
	gcc -c $? -o $@

tempfiles/menu-prerendering.o: src/menu-prerendering.c
	gcc -c $? -o $@

tempfiles/qw_utils.o: src/qw_utils.c
	gcc -c $? -o $@

tempfiles/scores.o: src/scores.c
	gcc -c $? -o $@

run: 2048
	./2048

fresh: clean 2048

clean:
	rm -rf tempfiles
	rm -f 2048
