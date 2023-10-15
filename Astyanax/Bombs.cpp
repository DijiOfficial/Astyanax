#include "pch.h"
#include "Bombs.h"
#include <iostream>

Bombs::Bombs(const Point2f& bottomLeft, const int horSpeed, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 8.f, horSpeed, 200.f, abs(int("bomb")), 1, texture, enemyType),
	m_Epsilon{ 0.8f },
	m_Bounces{ 0 },
	m_CanBounce{ true },
	m_Origin{ bottomLeft },
	m_CollisionPoint{ Point2f{ 0.f, 0.f } },
	m_BombState{ BombState::Bouncing }
{
}

void Bombs::Draw() const
{
	if (m_BombState == Bouncing) m_pTexture->Draw(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 });
	else
	{
		const Rectf srcRect{m_AnimFrame * 120.f , 0.f, 120.f, 120.f };
		const Rectf dstRect{ m_Shape.left - m_Shape.width, m_Shape.bottom - m_Shape.height , m_Shape.width * 4 , m_Shape.height * 4 };
		m_pBlastTexture->Draw(dstRect, srcRect);
	}

	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void Bombs::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);
	if (m_BombState == Bouncing and not m_IsFrozen)
	{
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;

		if (level->IsCollidingWithWorld(m_Shape)) m_Velocity.x = -m_Velocity.x;
		if (level->IsCollidingVertically(m_Shape) and m_CanBounce)
		{
			m_Bounces++;
			m_CollisionPoint = Point2f{ m_Shape.left, m_Shape.bottom };
			m_Origin.y = m_CollisionPoint.y + (m_Origin.y - m_CollisionPoint.y) * pow(m_Epsilon, 2);
			m_CanBounce = false;
			m_Velocity.y = -(m_Velocity.y * m_Epsilon);
		}
		if (m_Shape.bottom > m_Origin.y - (m_Origin.y - m_CollisionPoint.y) / 2.f and not m_CanBounce) m_CanBounce = true;
		if (m_Bounces >= 3)
		{
			m_BombState = BombState::Exploding;
			deathAudio->SetVolume(128);
			deathAudio->Play(0);
		}
	}
	else if (m_BombState == Exploding) UpdateFrames(elapsedSec);
	
}

void Bombs::UpdateFrames(const float elapsedSec)
{
	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 };
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 1.f / (m_NrFramesPerSec + 8))
	{
		++m_AnimFrame;
		m_AnimTime -= 1.f / (m_NrFramesPerSec + 8);
	}
	if (m_AnimFrame >= 9) 
	{
		m_AttackZone = Rectf{ 0.f, 0.f, 0.f, 0.f };
		m_BombState = Exploded;
	} 
}

Bombs::BombState Bombs::GetBombState() const
{
	return m_BombState;
}

Entity::ActionState Bombs::GetState() const
{
	switch (m_BombState)
	{
	case Entity::Bouncing:
	case Entity::Exploding:
		return ActionState::alive;
		break;
	case Entity::Exploded:
		return ActionState::dead;
		break;
	}
}

void Bombs::KillEntity()
{
	m_BombState = BombState::Exploded;
}
