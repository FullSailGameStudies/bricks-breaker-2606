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
	brick.clear();

	int theX = 0;
	int theWidth = 10;
	int theHeight = 2;
	int theY = 5;
	int theHealth = 3;

	for (int i = 0; i < 5; i++)
	{
		Box BrickTemp;

		BrickTemp.health = theHealth;
		BrickTemp.width = theWidth;
		BrickTemp.height = theHeight;
		BrickTemp.x_position = theX;
		BrickTemp.y_position = theY;
		BrickTemp.doubleThick = true;
		BrickTemp.color = ConsoleColor::DarkGreen;

		BrickTemp.x_position = theX + (i * theWidth);
		brick.push_back(BrickTemp);
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
	for (int i = 0; i < brick.size(); i++)
	{
		brick[i].Draw();
	}

	int midX = WINDOW_WIDTH / 2;
	int midY = WINDOW_HEIGHT / 2;

	if (brick.empty())
	{
		Console::SetCursorPosition(midX - 14, midY);
		std::cout << "You Win! Press 'R' to reset.";
	}
	else if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		Console::SetCursorPosition(midX - 14, midY);
		std::cout << "You Lose! Press 'R' to reset.";
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
		return;
	}
	if (brick.empty())
	{
		ball.moving = false;
		return;
	}
	for (int i = brick.size() - 1; i >= 0; i--)
	{
		int nextBallX = ball.x_position + ball.x_velocity;
		int nextBallY = ball.y_position + ball.y_velocity;


		if (brick[i].Contains(nextBallX, nextBallY))
		{
			ball.y_velocity *= -1;
			brick[i].color = (ConsoleColor)(brick[i].color - 1);


			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (brick[i].color == ConsoleColor::Black || brick[i].color == 0)
			{
				brick.erase(brick.begin() + i);
			}
			break;
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset

	if (brick.empty())
	{
		ball.moving = false;
		return;
	}
	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
		return;
	}
}
