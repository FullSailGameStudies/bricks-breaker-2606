#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	std::vector<Box> bricks;

	//Generates new rows of bricks when the player has destroyed all the bricks in the current row
	int rowsGenerated = 0;
	int maxRowsGenerated = 3;

	bool gameWon = false;
	bool gameLost = false;

public:
	Game();
	bool Update();
	void Render() const;
	void AddNewRow(int rowY);
	void Reset();
	void ResetBall();
	void CheckCollision();
};