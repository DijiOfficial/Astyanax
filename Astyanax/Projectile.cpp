#include "pch.h"
#include "Projectile.h"
#include <iostream>
Projectile::Projectile(const Point2f& bottomLeft, const int horSpeed, const int speed, const Texture* texture, const Entity::EnemyType enemyType, const int nrOfFrames) 
	: Entity(bottomLeft, 8.f, horSpeed, speed, abs(int("Projectile")), 1, texture, enemyType),
	m_ActionState{ ActionState::alive }
	, m_AnimFrame{ 0 }
	, m_AnimTime{ 0.f }
	, m_NrOfFrames{ nrOfFrames }
{
	if (nrOfFrames == 0)
		m_TextClip = Rectf{ 0, 0, m_Shape.width, m_Shape.height };
	else
	{
		m_TextClip = Rectf{ 0, 0, m_Shape.width * 2, m_Shape.height };
		m_Shape.width *= 2;
	}
}

void Projectile::Draw() const
{
	Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	Rectf dstRect{ 0.f, 0.f , m_TextClip.width * 2, m_TextClip.height * 2 };

	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		m_pTexture->Draw(dstRect, srcRect);
	}
	glPopMatrix();

	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void Projectile::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);

	if (not m_IsFrozen)
	{
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;

		if (m_Velocity.y >= m_Acceleration.y and m_Velocity.y != 0 and m_NrOfFrames == 0)
			m_Velocity.y += m_Acceleration.y * elapsedSec;		
	}

	if (level->IsCollidingVertically(m_Shape) or level->IsCollidingWithWorld(m_Shape)) KillEntity();
	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	UpdateFrames(elapsedSec);
}

void Projectile::KillEntity()
{
	m_ActionState = ActionState::dead;
}

Entity::ActionState Projectile::GetState() const
{
	return m_ActionState;
}

void Projectile::UpdateFrames(float elapsedSec)
{
	if (m_NrOfFrames == 0)
		return;

	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 0.25f / m_NrOfFrames)
	{
		++m_AnimFrame % m_NrOfFrames;
		m_AnimTime -= 0.25f / m_NrOfFrames;
	}
}