import std::io;
import sdl;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

fn void! main()
{
    sdl::init(sdl::INIT_VIDEO);
    sdl::Window* window = sdl::createWindow("Hello World SDL",
					     sdl::WINDOWPOS_UNDEFINED,
					     sdl::WINDOWPOS_UNDEFINED,
					     WINDOW_WIDTH,
					     WINDOW_HEIGHT,
					    (uint)sdl::WINDOW_SHOWN);


    sdl::Renderer* renderer = sdl::createRenderer(window, -1,
						  (uint)sdl::RENDERER_ACCELERATED);
    sdl::Event e;

    bool quit = false;
    sdl::Rect fillRect = { WINDOW_WIDTH / 4,
                           WINDOW_HEIGHT / 4,
                           WINDOW_WIDTH / 2,
                           WINDOW_HEIGHT / 2};

    while (!quit) {
	while (sdl::pollEvent(&e) != 0) {
	    if (e.type == sdl::QUIT) {
		quit = true;
	    }
	}
	// Clear screen
	sdl::setRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);  // White
	sdl::renderClear(renderer);

	// Draw red-filled square
	sdl::setRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red
	sdl::renderFillRect(renderer, &fillRect);

	// Update screen
	sdl::renderPresent(renderer);
    }
    sdl::destroyRenderer(renderer);
    sdl::destroyWindow(window);
    sdl::quit();
}
