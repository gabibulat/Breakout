#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "LevelXML.h"


struct Brick {
	char id;
	bool exists;
	SDL_Rect brickRect;
	int hitPoints; 
};

class Scene
{
public:
	Scene();
	void SetNewLevel();
	void Update(int paddleXposition, SDL_Point ballPosition);
	void Paused();
	void GameOver();

	
	int getBrickCount() { return brickCount; }
	//Const references 
	vector<Brick>& getBrickObjects() { return brickObjects; }
	const map<char, int>& getBreakScore() { return BreakScore; };
	const map<pair<char, string>, Mix_Chunk*>& getSounds() { return sounds; };
	const SDL_Point& getScreenSize() { return screenSize; };
	const int& getBallSize() { return ballSize; };
	const SDL_Rect& getPaddleRect() { return paddleRect; };

	//References, not const because it's updated in gameLogic
	int& getLevel() { return level; };
	int& getScore() { return score; };
	int& getNumberOfLives() { return numberOfLives; };

private:
	void DrawOnScreen(SDL_Texture* texture, SDL_Rect sourceRect);

	int level = 1;
	int score = 0;
	int numberOfLives = 4;

	const SDL_Point screenSize = { 1280,768};
	SDL_Renderer* renderer;
	
	TTF_Font* font;
	const int fontSize = 25;
	int lastScore = 0;

	SDL_Texture* backgroundTexture, * paddleTexture, * ballTexture, * lifeTexture, * textLevelTexture, * textScoreTexture, * textPauseTexture, * textGameOverTexture;
	map <char, SDL_Texture*> brickTexture;
	SDL_Rect backgroundRect, paddleRect, ballRect, lifeRect, textLevelRect, textScoreRect;
	int ballSize;
		
	int brickCount;
	map<char, int> BreakScore;
	vector<Brick> brickObjects;
	map<pair<char, string>, Mix_Chunk*> sounds;
	
};