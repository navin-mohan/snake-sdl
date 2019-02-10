#pragma once

#include <deque>
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


protected:
	// ordered list of coords representing
	// each of the body segment of the snake
	std::deque<SDL_Point> body_segments;
	
	// current direction of the snake
	SDL_Point current_direction;

	// rectangle defining the shape
	// of each body segment
	SDL_Point segment_block;

	// game score
	int score;


public:
	// all four possible directions
	const static SDL_Point directions[];
	
	static SDL_Point canvas_size;

	// location of the apple on the canvas
	static SDL_Point apple_location;

	// score for each apple
	static int score_per_apple;

	
	Snake(SDL_Point& start, int block_w, int block_h);

	// updates the state of the snake
	void update();

	// freeze the snake
	inline void freeze();

	// unfreeze the snake
	inline void unfreeze();

	// update the snake direction of movement
	inline void changeDirectionTo(Direction dir);
};


void Snake::changeDirectionTo(Direction dir) {
	current_direction = Snake::directions[dir];
}

void Snake::freeze() {
	frozen = true;
}

void Snake::unfreeze() {
	frozen = false;
}

