#include "Scene.h"

/*!
	Used for creating window, renderer, textures, filling brick struct with brick info form LevelXML & updating scene
*/

Scene::Scene()
{
	SDL_Window* window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, SDL_WINDOW_SHOWN);
	if (window == NULL) printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Initialize PNG loading, SDL_mixer, TTF
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

	}
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	paddleTexture = IMG_LoadTexture(renderer, "Images/paddle.png");
	ballTexture = IMG_LoadTexture(renderer, "Images/ball.png");
	lifeTexture = IMG_LoadTexture(renderer, "Images/heart.png");
	font = TTF_OpenFont("Font/DejaVuSansMono.ttf", fontSize);
	textPauseTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "PAUSED press p to continue", { 0xFF, 0xFF, 0xFF, 0xFF }));
	textGameOverTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "press SPACE to start new game", { 0xFF, 0xFF, 0xFF, 0xFF }));

	//Positions and sizes
	const int spacing = 10; //Custom spacing between screen edges or other elements

	//Background x,y,w,h
	backgroundRect = { 0,0, screenSize.x,screenSize.y };

	//TextScore y - changable in Update() x,w,h
	textScoreRect.y = screenSize.y - fontSize - spacing; //According to screen height, spacing and font from bottom edge of screen

	//Life y,w,h - x is changable in Update() 
	SDL_QueryTexture(lifeTexture, NULL, NULL, &lifeRect.w, &lifeRect.h);
	const int lifeYposition = screenSize.y - lifeRect.h - spacing;
	lifeRect.y = lifeYposition;

	//Paddle y,w,h - x is changable in Update()
	SDL_QueryTexture(paddleTexture, NULL, NULL, &paddleRect.w, &paddleRect.h);
	paddleRect.y = lifeYposition - paddleRect.h - spacing; //Right above lifes, level, score and spacing 

	//Ball w -  assuming it's circle w&h the same
	SDL_QueryTexture(ballTexture, NULL, NULL, &ballRect.w, &ballRect.h);
	ballSize = ballRect.w;

	textLevelRect = { spacing,textScoreRect.y }; //X position according to 0 (screen x) + spacin; Y poisition same as textScore
}

void Scene::DrawOnScreen(SDL_Texture* texture, SDL_Rect destinationRect)
{
	SDL_Rect sourceRect = { 0,0, destinationRect.w, destinationRect.h };
	SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
}

void Scene::Update(int paddleXposition, SDL_Point ballPosition)
{
	SDL_RenderClear(renderer);

	//Background
	DrawOnScreen(backgroundTexture, backgroundRect);

	//Ball
	ballRect.x = ballPosition.x; ballRect.y = ballPosition.y;
	DrawOnScreen(ballTexture, ballRect);

	//Score text - if score has changed then change texture to that score
	if (lastScore != score || score == 0) {
		SDL_DestroyTexture(textScoreTexture);
		textScoreTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, ("SCORE:" + to_string(score)).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF }));
		lastScore = score;
		SDL_QueryTexture(textScoreTexture, NULL, NULL, &textScoreRect.w, &textScoreRect.h);
		textScoreRect.x = screenSize.x - textScoreRect.w; //According to right edge of screen, score text width and spacing 
	}
	DrawOnScreen(textScoreTexture, textScoreRect);

	//Lives
	for (auto i = 1; i <= numberOfLives; i++) DrawOnScreen(lifeTexture, { lifeRect.x + lifeRect.w * i, lifeRect.y,lifeRect.w, lifeRect.h });

	//Paddle
	paddleRect.x = paddleXposition;
	DrawOnScreen(paddleTexture, paddleRect);

	//Level text
	DrawOnScreen(textLevelTexture, textLevelRect);

	//Bricks
	for (size_t i = 0; i < brickObjects.size(); i++) {
		if (brickObjects.at(i).exists == true) {
			DrawOnScreen(brickTexture[brickObjects.at(i).id], brickObjects.at(i).brickRect);
		}
	}

	SDL_RenderPresent(renderer);
}

//Setting the scene on a new level load
void Scene::SetNewLevel()
{
	//Load LevelsXML class
	LevelXML LevelXML(level);

	brickObjects.clear();

	//Load texture
	const string& backgroundTexturePath = LevelXML.getBackgroundTexturePath();
	backgroundTexture = IMG_LoadTexture(renderer, backgroundTexturePath.c_str());
	textLevelTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, ("LEVEL:" + to_string(level)).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF }));

	//Positions and sizes ; changable level number = changable starting position of lifes and levelText w,h 
	SDL_QueryTexture(textLevelTexture, NULL, NULL, &textLevelRect.w, &textLevelRect.h);
	lifeRect.x = textLevelRect.w; //According to Level text width - next to it


#pragma region Bricks
	int impenetrableCount = 0; // For counting how many bricks are left on screen - impenetrable bricks
	//Geting const references from LevelXML
	const string& bricksLayout = LevelXML.getBricksLayout();
	const int& columnSpacing = LevelXML.getColumnSpacing();
	const int& rowSpacing = LevelXML.getRowSpacing();
	const map<pair<string, string>, string>& brickInfo = LevelXML.getBrickInfo();
	const map<string, string>& brickTexturePath = LevelXML.getBrickTexturePath();

	SDL_Point defaultBrickSize; //Needed for brick size space "_"
	SDL_QueryTexture((IMG_LoadTexture(renderer, (brickTexturePath.begin()->second).c_str())), NULL, NULL, &defaultBrickSize.x, &defaultBrickSize.y);


	//Adjusting the bricks to be on the center of the screen
	int rowWidth = 0;
	int i = 1;
	while (bricksLayout[i] != '\n') {
		if (bricksLayout[i] == ' ') rowWidth += columnSpacing;
		else  rowWidth += defaultBrickSize.x;
		i++;
	}

	SDL_Point brickPosition = { (screenSize.x - rowWidth) / 2 ,30 };
	int j = 0;

	for (auto i = 0U; i < bricksLayout.length() - 1; i++) {
		switch (bricksLayout[i]) {
		case '\n':
			if (i != 0) {
				brickPosition.y = brickPosition.y + defaultBrickSize.y + rowSpacing;
				brickPosition.x = (screenSize.x - rowWidth) / 2;
			}
			break;
		case ' ': //Means space between bricks
			brickPosition.x = brickPosition.x + columnSpacing;
			break;
		case '_': //Means 'no brick'
			brickPosition.x = brickPosition.x + defaultBrickSize.x;
			break;
		default:
			Brick b;
			brickObjects.push_back(b);

			if (string(1, bricksLayout[i]) != "I") {

				brickObjects.at(j).hitPoints = stoi(brickInfo.at(make_pair(string(1, bricksLayout[i]), "HitPoints")));
				brickCount++;
			}

			brickObjects.at(j).id = bricksLayout[i];
			brickObjects.at(j).brickRect.x = brickPosition.x;
			brickObjects.at(j).brickRect.y = brickPosition.y;
			brickObjects.at(j).exists = true;

			if (brickTexture.find(bricksLayout[i]) == brickTexture.end()) {

				brickTexture[bricksLayout[i]] = IMG_LoadTexture(renderer, (brickTexturePath.at(string(1, bricksLayout[i]))).c_str());
				sounds[make_pair(bricksLayout[i], "HitSound")] = Mix_LoadWAV((brickInfo.at(make_pair(string(1, bricksLayout[i]), "HitSound"))).c_str());

				if (bricksLayout[i] != 'I') {

					BreakScore[bricksLayout[i]] = stoi(brickInfo.at(make_pair(string(1, bricksLayout[i]), "BreakScore")));
					sounds[make_pair(bricksLayout[i], "BreakSound")] = Mix_LoadWAV((brickInfo.at(make_pair(string(1, bricksLayout[i]), "BreakSound"))).c_str());

				}
			}
			SDL_QueryTexture(brickTexture.at(bricksLayout[i]), NULL, NULL, &brickObjects.at(j).brickRect.w, &brickObjects.at(j).brickRect.h);
			brickPosition.x = brickPosition.x + brickObjects.at(j).brickRect.w;
			j++;
		}
	}

	brickCount = brickObjects.size() - impenetrableCount;


#pragma endregion Bricks
}

void Scene::Paused()
{
	SDL_Point textPauseSize;
	SDL_QueryTexture(textPauseTexture, NULL, NULL, &textPauseSize.x, &textPauseSize.y);
	DrawOnScreen(textPauseTexture, { (screenSize.x / 2) - textPauseSize.x / 2 , screenSize.y / 2, textPauseSize.x,textPauseSize.y });
	SDL_RenderPresent(renderer);
}

void Scene::GameOver()
{
	numberOfLives = 4;
	score = 0;
	level = 1;
	SDL_Point textGameOverSize;
	SDL_QueryTexture(textGameOverTexture, NULL, NULL, &textGameOverSize.x, &textGameOverSize.y);
	DrawOnScreen(textGameOverTexture, { (screenSize.x / 2) - textGameOverSize.x / 2, screenSize.y / 2, textGameOverSize.x, textGameOverSize.y });
	SDL_RenderPresent(renderer);
}
