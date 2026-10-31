#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector

	brick.clear(); //gotta clear the boxes right? im a test with out if it would give a intresting challenge >:3

	for (int i = 0; i < 5;i++) // simple for loop, hopefully it works when i get the other errors fixed
	{
		Box brick;

		brick.width = 10;
		brick.height = 2;
		brick.x_position = i * 12; //i completly forgot that the x postion was the horizontal plain and was going to leave this alone ;-;
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkCyan; // ok changed the color aspect of it to actually dissapear in 3 hits
		
		Game::brick.push_back(brick); //me had a head ache when i did not incluse the Game:: at the start

	} //yippe! cleared the errors here! :D on to the next one!!!
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	if (GetAsyncKeyState('C') & 0x1) //i implemented this so that i dont have to wait for the ball to destroy all the bricks to show the victory message ;-;
		Game::brick.clear();

	if (GetAsyncKeyState('V') & 0x1) //i implemented a dev console command to test out the failure condition
		ball.y_position = WINDOW_HEIGHT - 1;


	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (const Box& brick : Game::brick) // well this was easy ... too easy *squinting eyes*
	{
		brick.Draw();
	}

	if (Game::brick.empty())
	{
		Console::ForegroundColor(ConsoleColor::White); //forgot to add notes on this on the previous commit ;-;
		Console::WordWrap(20, 15, 40, "You win! Press R to play again.");
		Console::ResetColor();
	} 
	
	else if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		Console::ForegroundColor(ConsoleColor::Red); //the falure condition message! :D
		Console::WordWrap(20, 15, 40, "You lose. Press R to play again.");
		Console::ResetColor();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	int removeindex = -1; //added these two to keep track of the boxes to delete later
	int index = 0;
	for (Box& brick : Game::brick) // simply implemented the colitoon back to the code as the first todo was stated :D (its still too easy *super sus eyes)
	{
		// TODO #4 - Update collision to check all bricks
		if (brick.Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick.color = ConsoleColor(brick.color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (brick.color == ConsoleColor::Black)
			{
				removeindex = index; // simple storage to track which block will turn black! :D
			}
			break;
		}
		++index;
	}
	if (removeindex != -1)
	{
		Game::brick.erase(Game::brick.begin() + removeindex); //bye bye brick! just deletes the brick that turns black without moving the others visually! :D
	}
	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (Game::brick.empty())
	{
		ball.moving = false; //tested and it works well now gotta make the message
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false; //this is the fail condion to make it stop, but if its forced (pressing v) while its moving upwards, it keeps going
	}
}
