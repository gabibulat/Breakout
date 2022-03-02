#include "Ball.h"

/*!
	Vector class for float pair for ball position and velocity. 
	Ball class for updating ball velocity and position.
*/

#pragma region VectorClass
Vec2::Vec2() : x(0.0f), y(0.0f) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}
#pragma endregion VectorClass

#pragma region BallClass
void Ball::SpawnBall(Vec2 position, Vec2 velocity, int ballSizeX)
{
	this->position = position;
	this->velocity = velocity;
	ballPosition.x = static_cast<int>(position.x);
	ballPosition.y = static_cast<int>(position.y);
	ballSize = ballSizeX;
}

void Ball::UpdatePosition(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	ballPosition.x = static_cast<int>(position.x);
	ballPosition.y = static_cast<int>(position.y);

}

void Ball::CollideWith(Contact  contact)
{
	// ball bouncing off  
	if (contact.type == CollisionType::BouncePaddleMiddle1)
	{
		velocity.x = -velocity.x;
		velocity.y = -bounceAngle1 * ballSpeed;
	}

	else if (contact.type == CollisionType::BouncePaddleCorner)
	{
		velocity.x = -velocity.x;
		velocity.y = -bounceAngle2 * ballSpeed;

	}

	else if (contact.type == CollisionType::BouncePaddleMiddle2 || contact.type == CollisionType::TopSideBrick) velocity.y = -bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::TopSideWall || contact.type == CollisionType::BottomSideBrick) velocity.y = bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::LeftSideWall || contact.type == CollisionType::RightSideBrick) velocity.x = bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::LeftSideBrick || contact.type == CollisionType::RightSideWall) velocity.x = -bounceAngle1 * ballSpeed;

}
#pragma endregion BallClass