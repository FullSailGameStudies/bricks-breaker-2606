#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	int brick_amount = 5;

	std::vector<Box*> bricks = {};

public:
	Game();
	~Game();
	bool Update();
	void Render() const;
	void ClearBricks();
	void Reset();
	void ResetBall();
	void CheckCollision();
};