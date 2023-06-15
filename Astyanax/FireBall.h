#pragma once
#include "Texture.h"
#include "utils.h"

class FireBall
{
public:
	FireBall(Point2f pos, Vector2f velocity);
	~FireBall() = default;
	FireBall(const FireBall& other) = delete;
	FireBall(FireBall&& other) noexcept = delete;
	FireBall& operator=(const FireBall& other) = delete;
	FireBall& operator=(FireBall&& other) noexcept = delete;
	void Update(float elapsedSec);
	void Draw() const;
	float GetDistance() const;
	Rectf GetFireBall() const;
private:
	const Texture m_FireBall;
	const Vector2f m_Velocity;
	const Point2f m_Origin;
	Rectf m_Shape;
};

