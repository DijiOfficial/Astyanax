#include "pch.h"
#include "PlantEnemy.h"

PlantEnemy::PlantEnemy(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 22, 0, 0, id, 45 ,texture, enemyType),
	m_ActionState{ ActionState::alive },
	m_HitAnimTime{ 0.f },
	m_TotalHitAnimTime{ 0.f },
	m_CanAttack{ true }
{
	m_TextClip = { Rectf{ 0.f, 0.f, 22.f, m_pTexture->GetHeight()} };
	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	m_NrOfFrames = 3;
	m_NrFramesPerSec = 3;
}

void PlantEnemy::Draw() const
{
	const Rectf srcRect{ m_AnimFrame * m_TextClip.width, m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	const Rectf dstRect{ m_Shape.left, m_Shape.bottom , m_Shape.width * 2, m_Shape.height * 2 };
	m_pTexture->Draw(dstRect, srcRect);
	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void PlantEnemy::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);
	
	if (m_ActionState != dead)
	{
		if (not m_IsFrozen)
		{
			if (m_AnimFrame == 2 and m_CanAttack)
			{
				m_CanAttack = false;
				m_ActionState = attacking;
			}
			else m_ActionState = alive;

			if (not m_CanAttack and m_AnimFrame == 0) m_CanAttack = true;
		}
		if (m_TotalHitAnimTime >= 0.6f)
		{
			m_IsHit = false;
			m_TotalHitAnimTime = 0;
			m_AnimFrame %= 3;
		}
	}
	
	
	UpdateFrames(elapsedSec);
}

void PlantEnemy::KillEntity()
{
	m_ActionState = ActionState::dead;
}

Entity::ActionState PlantEnemy::GetState() const
{
	return m_ActionState;
}

void PlantEnemy::UpdateFrames(const float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_IsHit)
	{
		m_HitAnimTime += elapsedSec;
		m_TotalHitAnimTime += elapsedSec;
		if (m_HitAnimTime >= 0.5f / (m_NrFramesPerSec + 8))
		{
			m_AnimFrame += 3;
			m_AnimFrame %= 9;
			m_HitAnimTime -= 0.5f / (m_NrFramesPerSec + 8);
		}
	}
	if (not m_IsFrozen)
	{
		if (m_AnimTime >= 1.f / m_NrFramesPerSec)
		{
			if (not m_IsFrozen) ++m_AnimFrame %= m_NrOfFrames;
			m_AnimTime -= 1.f / m_NrFramesPerSec;
		}
	}
}	