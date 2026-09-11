#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector> // include the vector class

//note to self : .h is is the declaration page, all the identifiers and functions will be declared here

class Game
{
	//There is bakk object and a Box object, both objects are child classes of Base Object. (See BaseObject Class)

	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	Box brick;

	std::vector<Box> bricks;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};