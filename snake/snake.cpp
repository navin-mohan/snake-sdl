#include "snake.hpp"
#include <ctime>
#include <cstdlib>

// static members

SDL_Point Snake::canvas_size;

SDL_Point Snake::apple_location;

int Snake::score_per_apple;

std::shared_ptr<SDL_Renderer> Snake::ren;

SDL_Point Snake::segment_block;



const SDL_Point Snake::directions[] = {
	{  0, -1 },  // NORTH
	{  0,  1 },  // SOUTH
	{ -1,  0 },  // WEST
	{  1,  0 }   // EAST 
};


void Snake::update() {

	// if the snake is frozen then do nothing
	if (frozen) return;
	
	// initializes the next block as the current first block
	SDL_Point next = body_segments.front();


	// synchronization
	direction_lock.lock();

	// update the next block according to 
	// the present direction of the snake
	next.x += ( current_direction.x * Snake::segment_block.x );
	next.y += ( current_direction.y * Snake::segment_block.y );

	direction_lock.unlock();

	if(next.x < 0)
		next.x += Snake::canvas_size.x;
	else
		next.x %= Snake::canvas_size.x;
	if(next.y < 0)
		next.y += Snake::canvas_size.y;
	else
		next.y %= Snake::canvas_size.y;

	// increment the body length if the snake 
	// hits an apple
	if (next == Snake::apple_location) {
		body_length++;
		score += Snake::score_per_apple;
		// update apple location
		Snake::placeApple(*this);
	} else {

		// check if the snake will hit itself
		for (const auto& segment : body_segments) {
			if (next == segment) {
				// resets the body length to 1 and score to 0
				body_length = 1;
				score = 0;
				break;
			}
		}
	}
	
	// insert the next block at the front
	body_segments.push_front(next);

	// remove any extra blocks that remains
	while (body_segments.size() > body_length) {
		body_segments.pop_back();
	}
}

Snake::Snake(SDL_Point& start, Color& _color)
	: score{ 0 }, frozen{ true }, color{ _color } {

	// insert the starting point as the first block
	body_segments.push_front(start);

	// set body length as body segment size
	body_length = body_segments.size();

}

void Snake::render() {
	SDL_SetRenderDrawColor(Snake::ren.get(), color.r, color.g, color.b, color.a);

	SDL_Rect rect;

	rect.h = Snake::segment_block.y;
	rect.w = Snake::segment_block.x;

	for (const auto& segment : body_segments) {
		rect.x = segment.x;
		rect.y = segment.y;
		SDL_RenderFillRect(Snake::ren.get(), &rect);
	}
}

// test equality of two SDL_Points
bool operator==(const SDL_Point& a, const SDL_Point& b) {
	return a.x == b.x && a.y == b.y;
}

void Snake::placeApple(const Snake& s) {
	SDL_Point apple;
	bool stop = false;
	
	const static int x_blocks = Snake::canvas_size.x / Snake::segment_block.x;
	const static int y_blocks = Snake::canvas_size.y / Snake::segment_block.y;

	srand((unsigned)time(0));
	
	while (!stop) {
		apple.x = (rand() % x_blocks) * Snake::segment_block.x;
		apple.y = (rand() % y_blocks) * Snake::segment_block.y;
		
		stop = true;

		for (const auto& segment : s.body_segments) {
			if (segment == apple) {
				stop = false;
				break;
			}
		}
	}

	Snake::apple_location = apple;
}



void handleEvent(Snake* s,bool& quit,std::mutex& mut) {
	SDL_Event e;
	//while(1)
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT: {
				mut.lock();
				quit = true;
				mut.unlock();
				return;
			}

			case SDL_KEYUP: {
				switch (e.key.keysym.sym) {
					case SDLK_UP: {
						s->changeDirectionTo(Direction::NORTH);
						break;
					}
					case SDLK_DOWN: {
						s->changeDirectionTo(Direction::SOUTH);
						break;
					}
					case SDLK_RIGHT: {
						s->changeDirectionTo(Direction::EAST);
						break;
					}
					case SDLK_LEFT: {
						s->changeDirectionTo(Direction::WEST);
						break;
					}
				}
			}
		}
	}
}