obj_files := 2048.o game.o get-input.o menu-rendering.o menus.o playfield-rendering.o

2048: tempfiles $(obj_files)
	gcc $(wildcard tempfiles/*.o) -o 2048

tempfiles:
	mkdir tempfiles -p

2048.o:
	gcc -c src/2048.c -o tempfiles/$@

game.o:
	gcc -c src/game.c -o tempfiles/$@

get-input.o:
	gcc -c src/get-input.c -o tempfiles/$@

menu-rendering.o:
	gcc -c src/menu-rendering.c -o tempfiles/$@

menus.o:
	gcc -c src/menus.c -o tempfiles/$@

playfield-rendering.o:
	gcc -c src/playfield-rendering.c -o tempfiles/$@

clear:
	rm -rf tempfiles
