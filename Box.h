#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int health = 3;
	int width = 2;
	int height = 2;
	void Draw() const override;
	bool Contains(int x, int y);
};