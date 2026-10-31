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
	bricks.clear();

	int initial_x = 0;
	int intial_y = 5;
	int brick_width = 10;
	int Brick_height = 2;

	for (int i = 0; i < 5; i++)
	{
		Box newBrick;
		newBrick.width = brick_width;
		newBrick.height = Brick_height;
		newBrick.x_position = initial_x + (i * brick_width);
		newBrick.y_position = intial_y;
		newBrick.doubleThick = true;
		newBrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(newBrick);
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
	for (const Box& b : bricks)
	{
		b.Draw();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (size_t i = 0; i < bricks.size(); i++)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.x_velocity *= -1;



			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == ConsoleColor::Black)
			{
				bricks.erase(bricks.begin() + i);
			}
			break;
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset


	if (bricks.empty())
	{	
		Console::ForegroundColor(ConsoleColor::Yellow);
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		std::cout << "VICTORY! Press R to Reset";
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	else if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		Console::ForegroundColor(ConsoleColor::Red);
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 11, WINDOW_HEIGHT / 2);
		std::cout << "GAME OVER! Press R to Reset";
	}
	Console::Lock(false);

}
