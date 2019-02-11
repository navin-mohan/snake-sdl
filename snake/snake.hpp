#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <SDL.h>


// test equality of two SDL_Points
bool operator==(const SDL_Point& a, const SDL_Point& b);

// color specified as 8-bit RGBA values 
struct Color {
	uint8_t r, g, b, a;
};


// Available directions
enum Direction {
	NORTH,
	SOUTH,
	WEST,
	EAST,
	COUNT
};

class Snake {

private:

	// keeps track of the current body length
	int body_length;

	// state of the snake
	// no movements when frozen
	bool frozen;

	// to synchronize direction
	std::mutex direction_lock;


protected:
	// ordered list of coords representing
	// each of the body segment of the snake
	std::deque<SDL_Point> body_segments;
	
	// current direction of the snake
	SDL_Point current_direction;

	// game score
	int score;

	// fill color for the snake 
	Color color;


public:
	// all four possible directions
	const static SDL_Point directions[];
	
	static SDL_Point canvas_size;

	// location of the apple on the canvas
	static SDL_Point apple_location;

	// score for each apple
	static int score_per_apple;

	// rectangle defining the shape
	// of each body segment
	static SDL_Point segment_block;

	// renderer for the snake
	static std::shared_ptr<SDL_Renderer> ren;

	// place the apple
	static void placeApple(const Snake& s);
	
	Snake(SDL_Point& start, Color& _color);

	// updates the state of the snake
	void update();

	// draws the snake
	void render();

	// freeze the snake
	inline void freeze();

	// unfreeze the snake
	inline void unfreeze();

	// update the snake direction of movement
	inline void changeDirectionTo(Direction dir);
};


void Snake::changeDirectionTo(Direction dir) {


	SDL_Point diff;

	if (body_length >= 2) {
		diff.x = (body_segments[1].x - body_segments[0].x) / Snake::segment_block.x;
		diff.y = (body_segments[1].y - body_segments[0].y) / Snake::segment_block.y;

		if (diff == Snake::directions[dir]) return;
	}

	direction_lock.lock();
	current_direction = Snake::directions[dir];
	direction_lock.unlock();
}

void Snake::freeze() {
	frozen = true;
}

void Snake::unfreeze() {
	frozen = false;
}

void handleEvent(Snake* s,bool& quit,std::mutex& mut);