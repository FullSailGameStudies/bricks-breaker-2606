#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	Ball ball;
	Box paddle;
	int victoryCondition = 0;
	Box textBox;
	char victoryMsg[100] = "Congrats, you've won. Press 'R' to reset.";
	char defeatMsg[100] = "Defeat, you missed the ball. Try again? Press 'R' to reset.";

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	std::vector<Box> bricks;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};