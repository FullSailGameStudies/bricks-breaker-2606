#include "stdafx.h"
#include "Game.h"
#include <vector>

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	defeat = false; // Reset defeat 

	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	brick.clear(); // Clears the vector from previous game attempt.



	// TODO #2 - Add this brick and 4 more bricks to the vector
	Box brick1;
	brick1.width = 10;
	brick1.height = 2;
	brick1.x_position = 0;
	brick1.y_position = 5;
	brick1.doubleThick = true;
	brick1.color = ConsoleColor::DarkGreen;
	brick.push_back(brick1);


	Box brick2;
	brick2.width = 10;
	brick2.height = 2;
	brick2.x_position = 12;
	brick2.y_position = 5;
	brick2.doubleThick = true;
	brick2.color = ConsoleColor::DarkGreen;
	brick.push_back(brick2);
	

	Box brick3;
	brick3.width = 10;
	brick3.height = 2;
	brick3.x_position = 24;
	brick3.y_position = 5;
	brick3.doubleThick = true;
	brick3.color = ConsoleColor::DarkGreen;
	brick.push_back(brick3);
	

	Box brick4;
	brick4.width = 10;
	brick4.height = 2;
	brick4.x_position = 36;
	brick4.y_position = 5;
	brick4.doubleThick = true;
	brick4.color = ConsoleColor::DarkGreen;
	brick.push_back(brick4);

	Box brick5;
	brick5.width = 10;
	brick5.height = 2;
	brick5.x_position = 48;
	brick5.y_position = 5;
	brick5.doubleThick = true;
	brick5.color = ConsoleColor::DarkGreen;
	brick.push_back(brick5);
	


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
	for (const auto& brick : brick)
	{
		brick.Draw();
	}
	bool victory = brick.empty();

	if (victory)
	{
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		Console::ForegroundColor(ConsoleColor::Yellow);
		std::cout << "Victory! you cleared the board! Press 'R' To Try Again";
	}

	

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (size_t i = 0; i < brick.size(); ++i)
	{
		if (brick[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick[i].color = ConsoleColor(brick[i].color - 1);
			ball.y_velocity *= -1;

			if (brick[i].color == ConsoleColor::Black)
			{
				brick.erase(brick.begin() + i);
				--i; // fixes the index when removing bricks from the vector
			}


			// TODO #5 - If the ball hits the same brick 2 times (color == black), remove it from the vector

			break;
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset

	if (brick.empty())
	{

		bool	victory = true;
		ball.moving = false; //freezes the ball when the win con is met.
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		Console::ForegroundColor(ConsoleColor::Yellow);
		std::cout << "Victory! you cleared the board! Press 'R' To Try Again";

	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}



	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT)
	{
		defeat = true;
		ball.moving = false;
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		Console::ForegroundColor(ConsoleColor::Red);
		std::cout << "Defeat you lost! Press 'R' To Try Again";
	}
}
