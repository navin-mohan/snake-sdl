// main.cpp : Defines the entry point for the application.
//


#include <iostream>
#include <SDL.h>
#include <memory>
#include <thread>
#include <mutex>
#include "snake.hpp"

#undef main


Snake* s;
std::mutex mut;
bool quit;


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	Snake::canvas_size = { 640,480 };
	Snake::segment_block = { 20,20 };
	Snake::score_per_apple = 1;

	
	SDL_Window* win;

	//TTF_Font* openSans = TTF_


	SDL_Surface* apple_img = nullptr;

	apple_img = SDL_LoadBMP("C:\\Users\\Navin Mohan\\Projects\\snake\\res\\apple.bmp");
	
	if (apple_img == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	win = SDL_CreateWindow("Snake", 100, 100, Snake::canvas_size.x, Snake::canvas_size.y + 100, SDL_WINDOW_SHOWN);

	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renptr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	if (renptr == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::shared_ptr<SDL_Renderer> ren(renptr);
	renptr = nullptr;
	
	Snake::ren = ren;

	SDL_Point start = { 620,240 };
	Color c = { 255,0,0,0 };
	s = new Snake(start,c);

	s->changeDirectionTo(Direction::EAST);
	
	s->unfreeze();
	int count = 50;
	quit = false;
	Snake::placeApple(*s);

	SDL_Texture* apple_tex = SDL_CreateTextureFromSurface(ren.get(), apple_img);

	SDL_FreeSurface(apple_img);

	SDL_Rect apple = {
		Snake::apple_location.x,
		Snake::apple_location.y,
		Snake::segment_block.x,
		Snake::segment_block.y
	};
	//std::thread eventHandler(handleEvent,s,quit,mut);
	mut.lock();
	while (!quit) {
		mut.unlock();
		SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 0);
		SDL_RenderClear(ren.get());
		s->render();
		SDL_SetRenderDrawColor(ren.get(), 0, 255, 0, 0);
		SDL_RenderCopy(ren.get(), apple_tex, NULL, &apple);
		SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);
		SDL_RenderDrawLine(ren.get(), 0, Snake::canvas_size.y, Snake::canvas_size.x, Snake::canvas_size.y);
		SDL_RenderPresent(ren.get());
		SDL_Delay(100);
		handleEvent(s, quit, mut);
		s->update();
		mut.lock();
		apple.x = Snake::apple_location.x;
		apple.y = Snake::apple_location.y;
		std::cout << apple.x << " " << apple.y << std::endl;
	}
	mut.unlock();

	//eventHandler.join();

	SDL_DestroyTexture(apple_tex);
	SDL_DestroyRenderer(renptr);
	SDL_DestroyWindow(win);
	SDL_Quit();

	delete s;
	return 0;
}
