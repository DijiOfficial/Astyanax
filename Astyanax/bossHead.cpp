#include "pch.h"
#include "bossHead.h"
#include <iostream>

BossHead::BossHead(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType)
	: Entity(bottomLeft, 32.f, 100.f, 0.f, id, 10000, texture, enemyType)
	, m_ActionState{ Entity::ActionState::spawning }
	, m_IdleTime{ 0.f }
	, m_IsAttacking{ false }
	, m_AnimeFrameCount{ 1 }
	, m_AttackingFrameCount{ 0 }
	, m_Radius{ 128.f }
	, m_OriginalAngle{ float(asin(38.f / m_Radius)) }
	, m_CircleCenter{ 0, 0 }
	, m_LastPosition{ 0 ,0 }
	, m_HitAnimTime{ 0.f }
	, m_TotalHitAnimTime{ 0.f }
{
	m_Angle = { m_OriginalAngle };
	m_TextClip = Rectf{ 0, 0, 32, 38 };
	m_Velocity.x = -m_HorSpeed;
	m_Velocity.y = -m_HorSpeed * 0.75;
}

void BossHead::Draw() const
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

void BossHead::Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio, const bool isTurning)
{
	const float screenWidth{ 576 };
	//make him go back and forth two if to avoid bobbling out of range because of floating point error
	if (m_Shape.bottom > m_Origin.y + 5) 
		m_Velocity.y = -m_HorSpeed * 0.75;
	else if (m_Shape.bottom < m_Origin.y - 5)
		m_Velocity.y = m_HorSpeed * 0.75;

	//head and body got out of sync once perhpas because of this?
	if (isTurning)
		m_Velocity.x = -m_Velocity.x;
	
	//reset attack state if there is one allows for 1 frame attack
	if (m_ActionState == ActionState::attacking or m_Angle >= 2 * M_PI + m_OriginalAngle)
	{
		if (m_ActionState == ActionState::jumping)
		{
			m_Shape.left = m_LastPosition.x;
			m_Shape.bottom = m_LastPosition.y;
		}

		m_Angle = m_OriginalAngle;
		m_AttackingFrameCount = 0;
		m_AnimeFrameCount = 0;
		m_IsAttacking = false;
		m_ActionState = ActionState::alive;
	}
	
	//hit animation
	if (m_TotalHitAnimTime >= 0.6f)
	{
		m_IsHit = false;
		m_TotalHitAnimTime = 0;
		m_AnimFrame %= 3;
	}

	//don't attack in first second
	if (not m_IsAttacking and m_AnimFrame == 0)
		m_IdleTime += elapsedSec;

	if (m_IdleTime >= 1.f and m_ActionState == ActionState::spawning)
	{
		m_ActionState = ActionState::alive;
		m_IdleTime = 0.f;
	}

	if (m_ActionState == ActionState::alive and m_IdleTime >= 0.3f)
	{
		if (rand() % 3 == 0) 
			m_IsAttacking = true;

		m_IdleTime -= 0.3f;
	}

	//m_IsAttacking replacing animation state
	if (m_IsAttacking and m_AttackingFrameCount == 2 and (m_ActionState != ActionState::jumping))
	{
		if (rand() % 2 == 0)
			m_ActionState = ActionState::attacking;
		else
		{
			m_ActionState = ActionState::jumping;
			m_CircleCenter = Point2f{ m_Shape.left - m_Radius, m_Shape.bottom - 38 };
			m_LastPosition = Point2f{ m_Shape.left, m_Shape.bottom };
		}
	}

	//jumping attack
	if (m_ActionState == ActionState::jumping)
	{
		m_Angle += 4 * elapsedSec ;
		if (m_Angle >= 2 * M_PI + m_OriginalAngle)
			m_Angle = 2 * M_PI + m_OriginalAngle;

		m_Shape.left = m_Radius * cos(m_Angle) + m_CircleCenter.x;
		m_Shape.bottom = m_Radius * sin(m_Angle) + m_CircleCenter.y;
	}

	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	UpdateFrames(elapsedSec);
	
	if (not m_IsAttacking)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
}

void BossHead::Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	if (m_ActionState == ActionState::dead)
		return;

	if (m_TotalHitAnimTime >= 0.6f)
	{
		m_IsHit = false;
		m_TotalHitAnimTime = 0;
		m_AnimFrame %= 3;
	}

	UpdateFrozenFrames(elapsedSec);
}

void BossHead::KillEntity()
{
	m_ActionState = ActionState::dead;
}

Entity::ActionState BossHead::GetState() const
{
	return m_ActionState;
}

void BossHead::UpdateFrames(const float elapsedSec)
{
	//hit animation
	if (m_IsHit)
	{
		m_HitAnimTime += elapsedSec;
		m_TotalHitAnimTime += elapsedSec;
		if (m_HitAnimTime >= 0.5f / (11))
		{
			m_AnimFrame += 3;
			m_AnimFrame %= 9;
			m_HitAnimTime -= 0.5f / (11);
		}
	}

	//attack over animation reset
	if (not m_IsAttacking and m_AnimFrame != 0)
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= 0.5f / 3)
		{
			--m_AnimFrame;
			m_AnimTime -= 0.5f / 3;
		}
	}

	//attack animation
	if (m_IsAttacking and (m_ActionState != ActionState::jumping and m_ActionState != ActionState::attacking))
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= 0.5f / 3)
		{
			if (m_AnimFrame == 2)
			{
				m_AnimeFrameCount = -1;
				m_AttackingFrameCount++;
			}
			else if (m_AnimFrame == 0)
				m_AnimeFrameCount = 1;

			if (m_AttackingFrameCount == 2)
				m_AnimFrame == 2;
			else
				m_AnimFrame += m_AnimeFrameCount;
			m_AnimTime -= 0.5f / 3;
		}
	}
}

void BossHead::UpdateFrozenFrames(const float elapsedSec)
{
	if (m_IsHit)
	{
		m_HitAnimTime += elapsedSec;
		m_TotalHitAnimTime += elapsedSec;
		if (m_HitAnimTime >= 0.5f / (11))
		{
			m_AnimFrame += 3;
			m_AnimFrame %= 9;
			m_HitAnimTime -= 0.5f / (11);
		}
	}
}
