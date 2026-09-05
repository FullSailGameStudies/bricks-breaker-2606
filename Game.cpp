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
	brick.clear();
	// TODO #2 - Add this brick and 4 more bricks to the vector
	for (int i = 0; i < 5; ++i)
	{
		Box newBrick;
		newBrick.width = 10;
		newBrick.height = 2;

		newBrick.x_position = i * 12;

		newBrick.y_position = 5;
		newBrick.doubleThick = true;
		newBrick.color = ConsoleColor::DarkGreen;
		brick.push_back(newBrick);
	}
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
	for (const Box& b : brick) {
		b.Draw();
	}
	if (brick.empty()) {
		std::cout << "VICTORY!Press R to reset";
	}
	else if (ball.y_position >= WINDOW_HEIGHT)
	{
		std::cout << "DEFEAT! Press r to reset";
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (auto it = brick.begin(); it != brick.end();){
		if (it->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			it->color = (ConsoleColor)(it->color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (it->color == ConsoleColor::Black)
			{
				it = brick.erase(it);
			}
			else {
				++it;
			}
		}
		else {
			++it;
		}

		

	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (brick.empty())
	{
		ball.moving = false;
		
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT) {
		ball.moving = false;
	}
}
