#include "pch.h"
#include "FireBall.h"

FireBall::FireBall(const Point2f pos, const Vector2f velocity) :
	m_FireBall{ Texture("fireball.png") },
	m_Velocity{velocity},
	m_Origin{ pos },
	m_Shape{Rectf{pos.x, pos.y, m_FireBall.GetWidth() * 2, m_FireBall.GetHeight() * 2}}
{

}

void FireBall::Update(const float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void FireBall::Draw() const
{
	m_FireBall.Draw(m_Shape);
}

float FireBall::GetDistance() const
{
	return 	utils::GetDistance(m_Origin, Point2f{m_Shape.left, m_Shape.bottom});
}

Rectf FireBall::GetFireBall() const
{
	return m_Shape;
}