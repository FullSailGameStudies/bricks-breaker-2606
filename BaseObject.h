#pragma once

class BaseObject
{ 

	// not to self : the base object contains a ConsoleColor enum, an X position and a Y position  and a "visage"?

public:

	ConsoleColor color = White;			
	int x_position = 0; //sets position x and y to 0
	int y_position = 0;
	char visage = '.'; // create a single char 

	//note to self : a pure virtual Draw class, will not modify the object -> see .cpp
	virtual void Draw() const;
};