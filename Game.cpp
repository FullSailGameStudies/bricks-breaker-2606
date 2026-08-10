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

	int brickCount = 5;
	int rowCount = 3;
	int brickWidth = 10;
	int brickSpacing = 0;
	int startingX = 0;
	int startingY = 5;
	int rowSpacing = 3;

	for (int row = 0; row < rowCount; row++)
	{
		for (int i = 0; i < brickCount; i++)
		{
			bricks.emplace_back();

			Box& brick = bricks.back();

			brick.width = brickWidth;
			brick.height = 2;

			brick.x_position = startingX + i * (brickWidth + brickSpacing);
			brick.y_position = startingY + row * rowSpacing;
			brick.doubleThick = true;
			brick.color = ConsoleColor::DarkCyan;
		}

	}
}


void Game::ResetBall()
{
	gameWon = false;
	gameLost = false;
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

void Game::AddNewRow(int rowY)
{
	int brickCount = 5;
	int brickWidth = 10;
	int brickSpacing = 1;
	int startingX = 0;

	for (int i = 0; i < brickCount; i++)
	{
		bricks.emplace_back();
		Box& brick = bricks.back();
		brick.width = brickWidth;
		brick.height = 2;
		brick.x_position = startingX + i * (brickWidth + brickSpacing);
		brick.y_position = rowY;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkCyan;
	}
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	//Sticks the ball to the paddle until Space is pressed to launch it
	if (!ball.moving && !gameLost)
	{
		ball.x_position = paddle.x_position + paddle.width / 2;
		ball.y_position = paddle.y_position - 1;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	int rowPositions[] = { 5, 8, 11 };
	for (int row = 0; row < 3; row++)
	{
		int rowY = rowPositions[row];
		bool rowHasBricks = false;
		for (const Box& brick : bricks)
		{
			if (brick.y_position == rowY)
			{
				rowHasBricks = true;
				break;
			}
		}
		if (!rowHasBricks && rowsGenerated < maxRowsGenerated)
		{
			AddNewRow(rowY);
			rowsGenerated++;
		}
	}
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
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i].Draw();
	}

	if (gameWon)
	{
		const char message[] = "You Win! Press 'R' to play again!";
		Console::SetCursorPosition((WINDOW_WIDTH - (sizeof(message) - 1)) / 2, WINDOW_HEIGHT / 2);
		std::cout << message;
	}
	else if (gameLost)
	{
		const char message[] = "You Lost. Press 'R' to play again!";
		Console::SetCursorPosition((WINDOW_WIDTH - (sizeof(message) - 1)) / 2, WINDOW_HEIGHT / 2);
		std::cout << message;
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (auto brick = bricks.begin(); brick != bricks.end();)
	{
		if (brick->Contains(ball.x_position + ball.x_velocity,
			ball.y_position + ball.y_velocity))
		{
			brick->color = ConsoleColor(brick->color - 1);
			ball.y_velocity *= -1;
		// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (brick->color == ConsoleColor::Black)
			{
				brick = bricks.erase(brick);
			}
			else
			{
				++brick;
			}
			break;
		}
		else
		{
			++brick;
		}

	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty() && rowsGenerated >= maxRowsGenerated)
	{
		gameWon = true;
		ball.moving = false;
		return;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset

	if (ball.y_position >= WINDOW_HEIGHT - 1 && ball.moving == true)
	{
	
		ball.moving = false;
		gameLost = true;
	}
}
