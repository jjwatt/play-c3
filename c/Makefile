hello-caca: hello-caca.c
	gcc hello-caca.c -o hello-caca -lcaca

loadimage3: loadimage3.c
	gcc loadimage3.c -O3 -o loadimage3 -lcaca -lImlib2

hello-sdl: hello-sdl.c
	gcc hello-sdl.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o ./hello-sdl

sdl-gravity-square: sdl-gravity-square.c
	gcc sdl-gravity-square.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o ./sdl-gravity-square

sdl-plasma: sdl-plasma.cc
	g++ -std=c++20 -Wall -Wextra -pedantic -Ofast sdl-plasma.cc -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o sdl-plasma

%: %.c
	gcc -O3 $< -o $@ -lcaca -lm
