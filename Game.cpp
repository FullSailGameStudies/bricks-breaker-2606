#include "stdafx.h"
#include "Game.h"
#include <cstring>

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
	// create a row of 5 bricks centered across the window
	bricks.clear();
	Box brick;
	brick.width = 10;
	brick.height = 2;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	for (int i = 0; i < 5; ++i)
	{
		brick.x_position = i * (brick.width + 2);
		brick.hits = 3;
		bricks.push_back(brick);
	}
	victory = false;
	defeat = false;
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
	for (const auto& brick : bricks)
	{
		brick.Draw();
	}

	// If the player has won, display victory text and instruction to reset
	if (victory)
	{
		const char* msg = "YOU WIN! Press R to reset";
		Console::ForegroundColor(ConsoleColor::Green);
		int x = (WINDOW_WIDTH - (int)strlen(msg)) / 2;
		int y = WINDOW_HEIGHT / 2;
		Console::SetCursorPosition(x, y);
		std::cout << msg;
		Console::ResetColor();
	}

	// If the player has lost, display defeat text and instruction to reset
	if (defeat)
	{
		const char* msg = "YOU LOSE! Press R to reset";
		Console::ForegroundColor(ConsoleColor::Red);
		int x = (WINDOW_WIDTH - (int)strlen(msg)) / 2;
		int y = WINDOW_HEIGHT / 2;
		Console::SetCursorPosition(x, y);
		std::cout << msg;
		Console::ResetColor();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (size_t i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[i].hits -= 1;
			ball.y_velocity *= -1;
			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].hits <= 0)
			{
				bricks[i].color = ConsoleColor::Black;
				bricks.erase(bricks.begin() + i);
				--i;
			}
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset

	if (bricks.empty())
	{
		ball.moving = false;
		victory = true;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
		defeat = true;
	}
}
