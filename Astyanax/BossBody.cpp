#include "pch.h"
#include "BossBody.h"

BossBody::BossBody(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType)
	: Entity(bottomLeft, 68.f, 100.f, 0.f, id, 10000, texture, enemyType)
	, m_CanTurn{ false }
	, m_IsTurning{ false }
	, m_ActionState{ Entity::ActionState::alive }
{
	m_TextClip = Rectf{ 0, 0, 64, 96 };
	m_Velocity.x = -m_HorSpeed;
}

void BossBody::Draw() const
{
	Rectf dstRect2, dstRect3;
	Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	Rectf dstRect{ 0.f, 0.f , m_TextClip.width * 2, m_TextClip.height * 2 };

	if (m_ActionState == Entity::ActionState::dying)
	{
		Rectf textClip = Rectf{ 0, 0, 120, 120 };
		srcRect = Rectf{ m_AnimFrame * textClip.width, textClip.bottom + textClip.width, textClip.width, textClip.width };
		if (m_AnimFrame >= 5)
			srcRect = Rectf{ (m_AnimFrame % 5) * textClip.width, textClip.bottom, textClip.width, textClip.width };

		dstRect = Rectf{ -10.f,						0.f ,								4 * textClip.width / 5, 4 * textClip.width / 5 };
		dstRect2 = Rectf{ -36.f + textClip.width,		7.f ,								4 * textClip.width / 5, 4 * textClip.width / 5 };
		dstRect3 = Rectf{ -23.f + textClip.width / 2, -23.f + 2 * textClip.height / 3 , 4 * textClip.width / 5, 4 * textClip.width / 5 };
	}
	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);

		if (m_ActionState != Entity::ActionState::dying and m_ActionState != Entity::ActionState::dead) m_pTexture->Draw(dstRect, srcRect);
		else
		{
			m_pBlastTexture->Draw(dstRect, srcRect);
			m_pBlastTexture->Draw(dstRect2, srcRect);
			m_pBlastTexture->Draw(dstRect3, srcRect);
		}
	}
	glPopMatrix();

	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void BossBody::Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	if (m_ActionState == ActionState::alive)
	{
		const float screenWidth{ 576 };
		m_IsTurning = false;
		//make him go back and forth
		if ((m_Shape.left < level->GetBoundaries().width + screenWidth * 0.5f or m_Shape.left + m_Shape.width * 2 >= level->GetBoundaries().width + screenWidth) and m_CanTurn)
		{
			m_Velocity.x = -m_Velocity.x;
			m_CanTurn = false;
			m_IsTurning = true;
		}

		//allow him to turn again in middle of path
		if (m_Shape.left + m_Shape.width > level->GetBoundaries().width + screenWidth * 0.75f and m_Shape.left + m_Shape.width < level->GetBoundaries().width + screenWidth * 0.75f + 5)
			m_CanTurn = true;

		m_Shape.left += m_Velocity.x * elapsedSec;
		m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	}
	
	UpdateFrames(elapsedSec);
}

void BossBody::KillEntity()
{
	m_ActionState = ActionState::dying;
	m_AnimFrame = 0;
	m_AnimTime = 0.f;
}

Entity::ActionState BossBody::GetState() const
{
	return m_ActionState;
}

void BossBody::UpdateFrames(const float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_ActionState == dying)
	{
		if (m_AnimTime >= 0.8f / (11))
		{
			++m_AnimFrame;
			m_AnimTime -= 0.8f / (11);
		}
		if (m_AnimFrame >= 9) m_ActionState = dead;
		return;
	}

	if (m_AnimTime >= 0.5f / 2)
	{
		++m_AnimFrame %= 2;
		m_AnimTime -= 0.5f / 2;
	}

}
