#include "pch.h"
#include "Statue.h"

Statue::Statue(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType) : Entity{ bottomLeft, 14.f, 0.f, 0.f, id, 15, texture, enemyType},
	m_ActionState{ ActionState::alive },
	m_HitTimer{ 0.f }
{
}

void Statue::Draw() const
{
	const Rectf dstRect{ m_Shape.left, m_Shape.bottom , m_Shape.width * 2, m_Shape.height * 2 };
	m_pTexture->Draw(dstRect);
	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void Statue::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	if (m_IsHit)
	{
		m_HitTimer += elapsedSec;
		if (m_HitTimer >= 0.5f) m_IsHit = false;
	}
}

void Statue::KillEntity()
{
	if (m_ActionState == ActionState::dying) m_ActionState = ActionState::dead;
	else m_ActionState = ActionState::dying;
}

Entity::ActionState Statue::GetState() const
{
	return m_ActionState;
}

void Statue::UpdateFrames(float elapsedSec)
{
}