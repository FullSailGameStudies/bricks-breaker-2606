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
	//brick.width = 10;
	//brick.height = 2;
	//brick.x_position = 0;
	//brick.y_position = 5;
	//brick.doubleThick = true;
	//brick.color = ConsoleColor::DarkGreen;


	// int value of i starts at 0, if it is less than or equal to 4, 
	// increase by one increment, then repeat till done.
	for (int i = 0; i <= 4; i++) {
		Box tempbrick;
		tempbrick.width = 10;
		tempbrick.height = 2;
		// Set the x position to the int value of i and multiply it by 12 allowing the 10 spaces needed,
		// then adding an extra 2 spaces for spacing between bricks 
		tempbrick.x_position = i * 12;
		tempbrick.y_position = 5;
		tempbrick.doubleThick = true;
		tempbrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(tempbrick);
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

	// int i starts at 0, as long as i less than bricks vector size, 
	// increase i, then draw the brick that is at the index of i
	for (int i = 0; i < bricks.size(); i++) {
		bricks[i].Draw();
	}

	// Print winning message to center of streen.
	if (bricks.empty()) {
		Console::SetCursorPosition(20, 15);
		std::cout << "You win! Press 'R' to play again.";
	}
	// Print losing message to center of streen.
	if (ball.y_position == WINDOW_HEIGHT - 1) {
		Console::SetCursorPosition(20, 15);
		std::cout << "You lose. Press 'R' to play again.";
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	
	// int i starts at 0, as long as i less than bricks vector size, 
	// increase i
	for (int i = 0; i < bricks.size(); i++) {

		// updated to use bricks vector with index of i
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			// updated to use bricks vector with index of i
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == ConsoleColor::Black) {
				bricks.erase(bricks.begin() + i);
				break;
			}
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty()) {
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position == WINDOW_HEIGHT - 1) {
		ball.moving = false;
	}
}
