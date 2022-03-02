#pragma once
#include "Ball.h"
#include "Scene.h"
#include <chrono> 
#include <random>
using namespace chrono;

class GameLogic
{
public:
	GameLogic();	
private:
	Contact CheckCollision(SDL_Point ballPosition, float paddlePositionX, float ballDirection);
	int brickCount=0;

	//References to scene objects
	Scene scene=Scene();
	vector<Brick>& brickObjects = scene.getBrickObjects(); 
	const map<char, int>& BreakScore = scene.getBreakScore();
	const map<pair<char, string>, Mix_Chunk*>& sounds = scene.getSounds();
	int& score = scene.getScore();
	
	//Sizes of textures
	const int& ballSize = scene.getBallSize();
	const SDL_Point& screenSize = scene.getScreenSize();
	const SDL_Rect& paddleRect = scene.getPaddleRect();
	
};
