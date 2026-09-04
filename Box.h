#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int width = 2;// the width of the box
	int height = 2; // the height of the box 
	void Draw() const override; // override the virtual function m
	bool Contains(int x, int y);
};