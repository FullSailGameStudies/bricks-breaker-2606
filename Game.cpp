#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

Game::~Game() 
{
	ClearBricks();
}

void Game::ClearBricks()
{
	for (Box* brick : bricks)
	{
		delete brick;
	}

	bricks.clear();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'o';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	ClearBricks();
	for (int i = 0; i < brick_amount; i++)
	{
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = 0 + (i * 10);
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkCyan;

		Box* brick_ptr = new Box(brick);
		bricks.push_back(brick_ptr);
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
	bool pause = false;
	Console::Lock(true);
	Console::Clear();
	
	if (bricks.size() == 0)
	{
		printf("You win! Press R to play again.");
		pause = true;
	}

	if (ball.y_position > 32)
	{
		printf("You lose. Press R to play again.");
		pause = true;
	}

	if (!pause)
	{
		paddle.Draw();
		ball.Draw();

		// TODO #3 - Update render to render all bricks
		if (bricks.size() > 0)
		{
			for (Box* brick : bricks)
			{
				brick->Draw();
			}
		}
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		Box* brick = bricks[i];

		if (brick->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick->color = ConsoleColor(brick->color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (brick->color == 0)
			{
				delete brick;
				bricks.erase(bricks.begin() + i);
			}
		}
	}
	

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.size() == 0)
	{
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position > 32)
	{
		ball.moving = false;
	}
}
