#pragma once
#include "BaseObject.h"

class Ball : public BaseObject
{
public:
	bool moving = false; //is the ball moving ?
	int x_velocity = 0; // ball speed and direction at x
	int y_velocity = 0;  // ball speed and direction at y
	void Update();
};