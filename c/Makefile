OS := $(shell uname -s)

ifeq ($(OS),Darwin)
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2
else
LDFLAGS = -lSDL2
endif

hello-sdl: hello-sdl.c
	gcc hello-sdl.c $(CFLAGS) $(LDFLAGS) -o ./hello-sdl

sdl-gravity-square: sdl-gravity-square.c
	gcc sdl-gravity-square.c $(CFLAGS) -Ofast $(LDFLAGS) -o ./sdl-gravity-square

# TODO: Fix this so it works on macOS, too
hello-caca: hello-caca.c
	gcc hello-caca.c -o hello-caca -lcaca

loadimage3: loadimage3.c
	gcc loadimage3.c -O3 -o loadimage3 -lcaca -lImlib2

%: %.c
	gcc -O3 $< -o $@ -lcaca -lm
