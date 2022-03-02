#include "GameLogic.h"

/*!
	Gameplay Logic class - collisions, new level, gameover, updating score..

*/

Contact GameLogic::CheckCollision(SDL_Point ballPosition, float paddlePositionX, float ballDirection)
{

	Contact contact{};

#pragma region wallCollision

	//Top wall
	if (ballPosition.y <= ballSize / 2) contact.type = CollisionType::TopSideWall;

	//Left wall
	else if (ballPosition.x <= ballSize / 2 && ballPosition.y > ballSize / 2 && (ballPosition.y + ballSize) <= screenSize.y) contact.type = CollisionType::LeftSideWall;

	//Right wall
	else if (ballPosition.x + ballSize >= (screenSize.x - ballSize / 2) && ballPosition.y <= screenSize.y) contact.type = CollisionType::RightSideWall;

	//Bottom wall
	else if (ballPosition.y > screenSize.y && ballPosition.y < (screenSize.y + ballSize * 2)) contact.type = CollisionType::ExitedScreen;

	if (contact.type != CollisionType::None)return contact;

#pragma endregion wallCollision

#pragma region paddleCollision
	float ballMiddleX = ballPosition.x + ballSize / 2;

	//Ball bounces of the paddle depending on which part of the paddle it hits, 4 parts of a paddle

	float part1 = paddlePositionX + paddleRect.w / 4, part2 = paddlePositionX + paddleRect.w / 4 * 2, part3 = paddlePositionX + paddleRect.w / 4 * 3;

	if (ballPosition.y + ballSize >= paddleRect.y && ballPosition.y + ballSize < paddleRect.y + paddleRect.h) {

		if ((ballPosition.x + ballSize) >= paddlePositionX && ballMiddleX < part1)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleCorner;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle2;
		}
		else if (ballMiddleX >= part1 && ballMiddleX < part2)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle1;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle2;
		}
		else if (ballMiddleX >= part2 && ballMiddleX < part3)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle2;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle1;
		}
		else if (ballMiddleX >= part3 && ballPosition.x <= (paddlePositionX + paddleRect.w))
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle2;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleCorner;
		}
	}
	if (contact.type != CollisionType::None)return contact;

#pragma endregion paddleCollision

#pragma region brickCollision


	for (int i = 0; i <= brickObjects.size() - 1; i++) {
		if (brickObjects.at(i).exists) {

			//top and bottom brick hit
			if ((ballPosition.x + ballSize) > brickObjects.at(i).brickRect.x
				&& (ballPosition.x) < (brickObjects.at(i).brickRect.x + brickObjects.at(i).brickRect.w)) {

				if ((brickObjects.at(i).brickRect.y + brickObjects.at(i).brickRect.h) >= ballPosition.y
					&& brickObjects.at(i).brickRect.y < ballPosition.y) contact.type = CollisionType::BottomSideBrick;

				else if ((ballPosition.y + ballSize) >= brickObjects.at(i).brickRect.y
					&& ballPosition.y < brickObjects.at(i).brickRect.y) contact.type = CollisionType::TopSideBrick;
			}

			//Left and right brick hit
			else if (ballPosition.y< (brickObjects.at(i).brickRect.y + brickObjects.at(i).brickRect.h)
				&& (ballPosition.y + ballSize)>brickObjects.at(i).brickRect.y) {

				if ((ballPosition.x + ballSize) >= brickObjects.at(i).brickRect.x
					&& (ballPosition.x + ballSize) < (brickObjects.at(i).brickRect.x + brickObjects.at(i).brickRect.w)) contact.type = CollisionType::LeftSideBrick;


				else if (ballPosition.x <= (brickObjects.at(i).brickRect.x + brickObjects.at(i).brickRect.w)
					&& ballPosition.x > brickObjects.at(i).brickRect.x) contact.type = CollisionType::RightSideBrick;

			}

			if (contact.type != CollisionType::None) {
				Mix_PlayChannel(-1, sounds.at(make_pair(brickObjects.at(i).id, "HitSound")), 0);
				brickObjects.at(i).hitPoints--;

				if (brickObjects.at(i).hitPoints == 0) {
					Mix_PlayChannel(-1, sounds.at(make_pair(brickObjects.at(i).id, "BreakSound")), 0);

					brickObjects.at(i).exists = false;
					score += BreakScore.at(brickObjects.at(i).id);
					brickCount--;
				}
				break;
			}
		}
	}
#pragma endregion brickCollision
	return contact;
}


GameLogic::GameLogic()
{
	SDL_Event event;
	SDL_Point mousePosition;
	Contact contact;
	Ball ball;
	float dt;
	bool running = true; //Game loop running
	bool ballStart = false; //If ball is released from paddle
	bool firstball = false; //Needed for skipping first paddle collision when ball is released 
	bool newGame = false; //New game indicator when it's gameOver  
	bool paused = false; //Is paused indicator
	int randomMinus; //For random ball start direction

	SDL_ShowCursor(0);
	scene.SetNewLevel();
	brickCount = scene.getBrickCount();
	int& numberOfLives = scene.getNumberOfLives();
	int& level = scene.getLevel();

	while (running) {
#pragma region events
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT)running = false; //Exit when X on window is pressed

			//On click release the ball from the paddle
			else if (event.type == SDL_MOUSEBUTTONDOWN && ballStart == false)
			{
				ballStart = true;
				firstball = true;
				srand(time(NULL));
				randomMinus = rand() % 2 + 1;
				if (randomMinus == 1) ball.SetVelocityx(-ball.ballSpeed);
				contact.type = CollisionType::BouncePaddleMiddle1;
			}
			//On P pause game
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_p) {
					paused = !paused;
					scene.Paused();
				}
				//On SPACE new game
				else if (event.key.keysym.sym == SDLK_SPACE && newGame == true)
				{
					scene.SetNewLevel();
					brickCount = scene.getBrickCount();
					newGame = false;
					paused = false;
					ballStart = false;

				}
				//For testing purposes on B pressed new level, brickCount=0
				else if (event.key.keysym.sym == SDLK_b) {
					brickCount = 0;
				}
			}
		}
#pragma endregion events

		if (!paused) {
			auto startTime = high_resolution_clock::now();

			//Paddle following cursor
			SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
			if (mousePosition.x < 1) mousePosition.x = 1; //Left wall
			else if (mousePosition.x > screenSize.x - paddleRect.w) mousePosition.x = screenSize.x - paddleRect.w; //Right wall
			scene.Update(mousePosition.x, ball.getBallPosition());

			if (ballStart) {
				if (!firstball)contact = CheckCollision(ball.getBallPosition(), mousePosition.x, ball.getVelocity().x);
				ball.CollideWith(contact);

				if (contact.type == CollisionType::ExitedScreen) {
					numberOfLives--;
					scene.Update(mousePosition.x, ball.getBallPosition());
					ballStart = false;
				}

				if (numberOfLives == 0 || (brickCount == 0 && level == 3)) {
					scene.GameOver();
					newGame = true;
					paused = true;
					
					ballStart = false;
				}

				else if (brickCount == 0 && level!=3) {
					level++;
					contact.type = CollisionType::ExitedScreen;
					ball.CollideWith(contact);
					scene.SetNewLevel();
					brickCount = scene.getBrickCount();
					ballStart = false;
				}

				auto stopTime = high_resolution_clock::now();
				dt = duration<float, milliseconds::period>(stopTime - startTime).count();
				ball.UpdatePosition(dt);
				firstball = false;
			}
			else ball.SpawnBall(Vec2(mousePosition.x + paddleRect.w / 2 - ballSize / 2, paddleRect.y - ballSize), Vec2(ball.ballSpeed, ball.ballSpeed), ballSize);
		}
	}
}

