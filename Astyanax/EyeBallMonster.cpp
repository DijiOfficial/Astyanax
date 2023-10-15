#include "pch.h"
#include "EyeBallMonster.h"
#include <stdexcept>
#include <iostream>

EyeBallMonster::EyeBallMonster(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 24.f, 0.f, 300.f, id, 25, texture, enemyType),
	m_ActionState{ alive },
	m_HitAnimTime{ 0 },
	m_IsMovingVertically{ false },
	m_LockVerticalMovement{false},
	m_NewVerticalPosition{0}
{
	m_TextClip = { Rectf{ 0.f, 0.f, 24.f, m_pTexture->GetHeight()} };
	m_IsGoingLeft = false;
}

void EyeBallMonster::Draw() const
{
	const Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	const Rectf dstRect{ m_Shape.left, m_Shape.bottom - 2 * m_Shape.height / 3, m_Shape.width * 2, m_Shape.height * 2 };
	m_pTexture->Draw(dstRect, srcRect);
	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , 4 * m_Shape.height / 3 });
	}
}

void EyeBallMonster::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);

	if (m_IsHit)
	{
		if (not m_IsFrozen and m_ActionState != isHit) m_IsGoingLeft = !m_IsGoingLeft;
		m_ActionState = isHit;
	}
	
	if (m_ActionState != dead)
	{
		if (m_ActionState != isHit and not m_IsFrozen)
		{
			if (DistanceFromAvatar(avatar) <= (avatar.width * 2)) m_LockVerticalMovement = false;
			if (DistanceFromAvatar(avatar) >= (avatar.width * 10))
			{
				if (not m_LockVerticalMovement) m_IsMovingVertically = true;
				if (m_Shape.left >= avatar.left)
					m_IsGoingLeft = true;
				else
					m_IsGoingLeft = false;
			}
		}
		if (m_HitAnimTime >= 0.6)
		{
			m_ActionState = alive;
			m_IsHit = false;
			m_HitAnimTime = 0;
			m_AnimFrame %= 2;
		}
	}

	if (not m_IsFrozen)
	{
		if (not m_IsMovingVertically)
		{
			if (m_IsGoingLeft) m_Shape.left -= m_Velocity.x * elapsedSec;
			else m_Shape.left += m_Velocity.x * elapsedSec;
			if (level->IsCollidingWithWorld(m_Shape)) // avoid getting stuck on walls
			{
				m_Shape.bottom += m_Velocity.y * elapsedSec;
			}
		}
		else
		{
			if (m_NewVerticalPosition == 0) m_NewVerticalPosition = rand() % int(level->GetBoundaries().height - avatar.bottom - m_Shape.height) + avatar.bottom;
			else if ((m_Shape.bottom <= m_NewVerticalPosition + 3 and m_Shape.bottom >= m_NewVerticalPosition - 3) or level->IsCollidingVertically(m_Shape))
			{
				m_IsMovingVertically = false;
				m_LockVerticalMovement = true;
				m_NewVerticalPosition = 0;
			}
			else if (m_Shape.bottom < m_NewVerticalPosition) m_Shape.bottom += m_Velocity.y * elapsedSec;
			else if (m_Shape.bottom > m_NewVerticalPosition) m_Shape.bottom -= m_Velocity.y * elapsedSec;
		}
	}

	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , 4 * m_Shape.height / 3 };
	UpdateFrames(elapsedSec);
}

void EyeBallMonster::UpdateFrames(const float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_ActionState == isHit) 
	{
		m_HitAnimTime += elapsedSec;
		if (m_AnimTime >= 0.5f / (m_NrFramesPerSec + 10))
		{
			m_AnimFrame += 2;
			m_AnimFrame %= 6;
			m_AnimTime -= 0.5f / (m_NrFramesPerSec + 10);
		}
	}
	else if (not m_IsFrozen)
	{
		if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
		{
			if (not m_IsFrozen) ++m_AnimFrame %= m_NrOfFrames;
			m_AnimTime -= 0.5f / m_NrFramesPerSec;
		}
	}
}

Entity::ActionState EyeBallMonster::GetState() const
{
	return m_ActionState;
}

void EyeBallMonster::KillEntity()
{
	m_ActionState = Entity::dead;
}