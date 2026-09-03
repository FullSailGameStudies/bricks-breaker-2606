#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int width = 2;
	int height = 2;
	int hits = 3; 
	void Draw() const override;
	bool Contains(int x, int y);
};