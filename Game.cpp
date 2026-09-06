#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::SetBufferSize(WINDOW_WIDTH, WINDOW_HEIGHT);
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
	brickHits.clear();
	const int brickWidth = 10;
	const int brickHeight = 2;
	const int columnWidth = WINDOW_WIDTH / 5;

	for (int i = 0; i < 5; i++) {
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = i * columnWidth + (columnWidth - brickWidth) / 2; // even spacing
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;

		bricks.push_back(brick);
		brickHits.push_back(0);
	}

	gameOver = false;
	gameWon = false;
	
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
	
	for (auto const& brick : bricks) {
		brick.Draw();
	}
	
	// game win/loss conditions
	if (gameWon) {
		Console::ForegroundColor(ConsoleColor::White);
		std::string message = "You win! Press 'R' to play again.";
		Console::SetCursorPosition((WINDOW_WIDTH - (int)message.length()) / 2, WINDOW_HEIGHT / 2);
		std::cout << message;
	}
	else if (gameOver) {
		Console::ForegroundColor(ConsoleColor::White);
		std::string  message = "You lose! Press 'R' to play again.";
		Console::SetCursorPosition((WINDOW_WIDTH - (int)message.length()) / 2, WINDOW_HEIGHT / 2);
		std::cout << message;
	}

	Console::Lock(false);

}
 

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	
	for (int i = 0; i < bricks.size(); i++) {
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity)) {
			ball.y_velocity *= -1;
			brickHits[i]++;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector

			if (brickHits[i] >= 3) {
				bricks[i].color = ConsoleColor::Black;
				bricks.erase(bricks.begin() + i);
				brickHits.erase(brickHits.begin() + i);
			}
			
			else if (brickHits[i] == 2) {
				bricks[i].color = ConsoleColor::Red;
			}
			else {
				bricks[i].color = ConsoleColor::Yellow;
			}

			


			break;
		}
	}

	if (bricks.empty() && !gameWon) {
		gameWon = true;
		ball.moving = false;
	}

	if (ball.y_position >= WINDOW_HEIGHT - 1 && !gameOver) {
		gameOver = true;
		ball.moving = false;
	}
	
	

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
}
