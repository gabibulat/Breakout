#pragma once
#include <SDL_rect.h>

enum class CollisionType
{
	None,
	BouncePaddleCorner, BouncePaddleMiddle1, BouncePaddleMiddle2,
	TopSideWall, RightSideWall, LeftSideWall, ExitedScreen,
	RightSideBrick, LeftSideBrick, TopSideBrick, BottomSideBrick
};

struct Contact { CollisionType type; };

class Vec2
{
public:
	Vec2();
	Vec2(float x, float y);
	float x, y;

};


class Ball
{
public:
	const float ballSpeed = 0.5f;

	void SpawnBall(Vec2 position, Vec2 velocity, int ballSizeX);
	void UpdatePosition(float dt);
	void CollideWith(Contact contact);
	Vec2 getVelocity() { return velocity; };
	void SetVelocityx(float x) { velocity.x = x; };
	SDL_Point getBallPosition() { return ballPosition; };

private:
	SDL_Point ballPosition;
	const float bounceAngle1 = 0.75f, bounceAngle2 = 0.45f;
	int ballSize;
	Vec2 position;
	Vec2 velocity;
};

