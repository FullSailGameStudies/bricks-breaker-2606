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
	Box brick;
	brick.width = 10;
	brick.height = 2;
	brick.x_position = 0;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);
	brick.x_position = 20;
	brick.y_position = 3;
	bricks.push_back(brick);
	brick.x_position = 60;
	brick.y_position = 5;
	bricks.push_back(brick);
	brick.x_position = 35;
	brick.y_position = 0;
	bricks.push_back(brick);
	brick.x_position = 25;
	brick.y_position = 9;
	bricks.push_back(brick);

	textBox.x_position = 10;
	textBox.y_position = 10;
	textBox.width = 40;
	textBox.height = 10;
	textBox.doubleThick = true;
	textBox.color = ConsoleColor::Cyan;

	victoryCondition = 0;
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
	for (int brickNum = 0; brickNum < bricks.size(); brickNum++) {
		bricks[brickNum].Draw();
	}

	if (victoryCondition == 1) {
		textBox.Draw();
		Console::WordWrap(textBox.x_position + 2, textBox.y_position + 2, textBox.width - 2, victoryMsg);
	}
	else if (victoryCondition == 2) {
		textBox.Draw();
		Console::WordWrap(textBox.x_position + 2, textBox.y_position + 2, textBox.width - 2, defeatMsg);
	}
	

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int brickNum = 0; brickNum < bricks.size(); brickNum++) {
		if (bricks[brickNum].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[brickNum].color = ConsoleColor(bricks[brickNum].color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[brickNum].color == Black) {
				bricks.erase(bricks.begin() + brickNum);
				brickNum--;
			}
		}
	}
	
	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty()) {
		ResetBall();
		victoryCondition = 1;
	}


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}
	else if (ball.y_position == 30 && !paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position)) {
		ResetBall();
		victoryCondition = 2;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
}
