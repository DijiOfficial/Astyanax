#include "pch.h"
#include "Blob.h"
#include <iostream>

Blob::Blob(const Point2f& bottomLeft, const int id, const float speed, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 16.f, 0, speed, id, 1, texture, enemyType),
	m_ActionState{ alive }
{
	m_TextClip = { Rectf{ 0.f, 0.f, 16.f, m_pTexture->GetHeight()} };
}

void Blob::Draw() const
{
	const Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	const Rectf dstRect{m_Shape.left, m_Shape.bottom , m_Shape.width * 2, m_Shape.height * 2 };
	m_pTexture->Draw(dstRect, srcRect);
	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void Blob::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);
	if (not m_IsFrozen)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		if (m_Shape.bottom <= m_Origin.y - m_Shape.height or m_Shape.bottom >= m_Origin.y + m_Shape.height) m_Velocity.y = -m_Velocity.y;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		
		UpdateFrames(elapsedSec);
		m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	}
	if (abs(m_Shape.left - m_Origin.x) > 700.f) KillEntity();
}

void Blob::UpdateFrames(const float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
	{
		++m_AnimFrame %= m_NrOfFrames;
		m_AnimTime -= 0.5f / m_NrFramesPerSec;
	}
}

Entity::ActionState Blob::GetState() const
{
	return m_ActionState;
}

void Blob::KillEntity()
{
	m_ActionState = Entity::dead;
}
