#include "pch.h"
#include "PowerUp.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

PowerUp::PowerUp(const Point2f& bottomLeft, const PowerType power) :
	m_pTexture{new Texture{"PickUpsFinal.png"}},
	m_TextClip{0.f, 0, 8.f, 16.f},
	m_Velocity{Vector2f{0.0f, 400.0f}},
	m_Acceleration{Vector2f{0, -981.0f}},
	m_PowerType{power}
{
	m_Shape = Rectf{ bottomLeft.x, bottomLeft.y, 8.f, 16.f };
}

PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Update(const float elapsedSec, Level*& level)
{
	MovePowerUp(elapsedSec);
	level->HandleCollision(m_Shape, m_Velocity);
}

void PowerUp::Draw() const
{
	const Rectf dstRect{ m_Shape.left, m_Shape.bottom, m_TextClip.width * 2, m_TextClip.height * 2 };
	const Rectf srcRect{ m_TextClip.left + m_TextClip.width * m_PowerType, m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	m_pTexture->Texture::Draw(dstRect, srcRect);
}

bool PowerUp::IsOverlapping(const Rectf& rect) const
{
	return (utils::IsOverlapping(rect, m_Shape));
}

void PowerUp::MovePowerUp(const float elapsedSec)
{
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
}

PowerUp::PowerType PowerUp::GetPowerType() const
{
	return m_PowerType;
}