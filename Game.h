#pragma once
#include "Box.h"
#include "Ball.h"

#include <vector> // include vectore

class Game
{
	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	std::vector<Box> bricks; // store vector of bricks rather than just one
	std::vector<int> brickHits; // track hit count

	// needed for win/loss conditions
	bool gameOver = false;
	bool gameWon = false;


public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};