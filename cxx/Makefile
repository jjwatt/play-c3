OS := $(shell uname -s)

ifeq ($(OS),Darwin)
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2
else
LDFLAGS = -lSDL2
endif

sdl-plasma: sdl-plasma.cc
	g++ -std=c++20 -Wall -Wextra -pedantic -Ofast sdl-plasma.cc $(CFLAGS) $(LDFLAGS) -o sdl-plasma

sdl-gravity-square: sdl-gravity-square.cc
	g++ -std=c++20 -Wall -Wextra -pedantic -Ofast sdl-gravity-square.cc $(CFLAGS) $(LDFLAGS) -o sdl-gravity-square
