#include "snake.hpp"

const SDL_Point Snake::directions[] = {
	{  0,  1 },  // NORTH
	{  0, -1 },  // SOUTH
	{ -1,  0 },  // WEST
	{  1,  0 }   // EAST 
};


void Snake::update() {

	// if the snake is frozen then do nothing
	if (frozen) return;
	
	// initializes the next block as the current first block
	SDL_Point next = body_segments.front();

	// update the next block according to 
	// the present direction of the snake
	next.x += ( current_direction.x * segment_block.x ) % Snake::canvas_size.x;
	next.y += ( current_direction.y * segment_block.y ) % Snake::canvas_size.y;


	// increment the body length if the snake 
	// hits an apple
	if (next == Snake::apple_location) {
		body_length++;
		score += Snake::score_per_apple;
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

Snake::Snake(SDL_Point& start, int block_w, int block_h)
	: score{ 0 }, frozen{ true } {

	// insert the starting point as the first block
	body_segments.push_front(start);

	// set body length as body segment size
	body_length = body_segments.size();

	// initialize segment block
	segment_block = { block_w, block_h };


}



// test equality of two SDL_Points
bool operator==(const SDL_Point& a, const SDL_Point& b) {
	return a.x == b.x && a.y == b.y;
}