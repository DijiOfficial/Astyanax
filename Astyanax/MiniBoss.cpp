#include "pch.h"
#include "MiniBoss.h"
#include <iostream>

MiniBoss::MiniBoss(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType) :
	Entity{ bottomLeft, 32.f, 280.f, 0.f, id, 150, texture, enemyType },
	m_ActionState{ ActionState::spawning },
	m_HitAnimTime{ 0.f },
	m_TotalHitAnimTime{ 0.f },
	m_OriginalHeight{ m_pTexture->GetHeight() },
	m_CanBounce{ true },
	m_Bounces{ 0 },
	m_Origin{ Point2f{ bottomLeft.x, 400 } },
	m_CollisionPoint{ Point2f{ 0.f, 0.f } },
	m_IdleTime{ 0.f },
	m_TotalIdleTime{ 0.f }
{
	m_TextClip = Rectf{ 0.f, 0.f, 32.f, m_pTexture->GetHeight()};
	m_NrFramesPerSec = 3;
	m_NrOfFrames = 3;
	m_AnimFrame = 0;
}

void MiniBoss::Draw() const
{
	if (m_ActionState != deactivated)
	{
		const Rectf srcRect{ m_AnimFrame * m_TextClip.width, m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
		const Rectf dstRect{ 0, 0 , m_Shape.width * 2, m_OriginalHeight * 2 };


		glPushMatrix();
		{
			if (not m_IsGoingLeft)
			{
				glScalef(-1, 1, 1);
				glTranslatef(-m_Shape.left - m_Shape.width * 2, m_Shape.bottom, 0);
			}
			else glTranslatef(m_Shape.left, m_Shape.bottom, 0);
			m_pTexture->Draw(dstRect, srcRect);
		}
		glPopMatrix();

		if (m_Debugger.m_ShowHitBox)
		{
			utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
			utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
		}
	}
}

void MiniBoss::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);
	if (m_ActionState != deactivated and m_ActionState != dead)
	{
		if (not m_IsFrozen)
		{
			if (m_Shape.left >= avatar.left) m_IsGoingLeft = true;
			else m_IsGoingLeft = false;

			if (m_IsHit and m_ActionState != spawning and m_Bounces >= 3)
			{
				m_ActionState = isHit;
			}

			//attacking phase
			if (m_AnimFrame % 3 == 2)
			{
				m_ActionState = idle;
				m_IdleTime += elapsedSec;
				m_TotalIdleTime += elapsedSec;
				if (m_IdleTime >= 0.3f)
				{
					if (rand() % 2 == 0) m_ActionState = attacking;
					m_IdleTime -= 0.3f;
				}
				if (m_TotalIdleTime >= 2.01f)
				{
					m_AnimFrame = 0;
					m_ActionState = jumping;
					m_Shape.bottom += 1.f;
					m_TotalIdleTime = 0.f;
					m_CanBounce = true;
				}
			}

			if (m_ActionState == spawning)
			{
				m_Velocity.x = 0;
				if (m_Bounces >= 2)
				{
					m_ActionState = jumping;
				}
				if (level->IsCollidingVertically(m_Shape) and m_CanBounce)
				{
					m_Bounces++;
					m_CollisionPoint = Point2f{ m_Shape.left, m_Shape.bottom };
					m_Origin.y = m_CollisionPoint.y + (m_Origin.y - m_CollisionPoint.y);
					m_CanBounce = false;
					m_Velocity.y = 500.f;
				}
				if (m_Shape.bottom > m_Origin.y - (m_Origin.y - m_CollisionPoint.y) / 2.f and not m_CanBounce) m_CanBounce = true;
			}

			if (m_ActionState == jumping)
			{
				if (level->IsCollidingWithWorld(m_Shape)) m_Velocity.x = -m_Velocity.x;
				if (level->IsCollidingVertically(m_Shape) and m_CanBounce)
				{
					m_Bounces++;
					const float RandomSpeedModulator{ (rand() % 10 + 5) / 10.f};
					if (m_IsGoingLeft) m_Velocity.x = -m_HorSpeed * RandomSpeedModulator;
					else m_Velocity.x = m_HorSpeed * RandomSpeedModulator;

					m_CollisionPoint = Point2f{ m_Shape.left, m_Shape.bottom };
					m_Origin.y = m_CollisionPoint.y + (m_Origin.y - m_CollisionPoint.y);
					m_CanBounce = false;
					m_Velocity.y = 600.f;
				}
				if (m_Shape.bottom > m_Origin.y - (m_Origin.y - m_CollisionPoint.y) / 2.f and not m_CanBounce) m_CanBounce = true;

			}

			if (m_ActionState == isHit)
			{
				if (level->IsCollidingVertically(m_Shape))
				{
					m_ActionState = alive;
					m_Bounces = 0;
				}
			}

			m_Shape.left += m_Velocity.x * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
		}
	
		if (m_TotalHitAnimTime >= 0.6f)
		{
			m_IsHit = false;
			m_TotalHitAnimTime = 0;
			m_AnimFrame %= 3;
		}

		UpdateFrames(elapsedSec);
		ChangeShape();
		if (m_ActionState != jumping and m_ActionState != spawning) level->HandleCollision(m_Shape, m_Velocity);
		m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 };
	}
	else
	{
		m_AttackZone = Rectf{ 0.f, 0.f, 0.f, 0.f };
	}
}

void MiniBoss::KillEntity()
{
	if (m_ActionState == dead or m_ActionState == deactivated) m_ActionState = ActionState::deactivated;
	else m_ActionState = ActionState::dead;
}

Entity::ActionState MiniBoss::GetState() const
{
	return m_ActionState;
}

void MiniBoss::UpdateFrames(const float elapsedSec)
{
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
	if (not m_IsFrozen and m_ActionState == alive)
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= 1.f / m_NrFramesPerSec)
		{
			++m_AnimFrame %= m_NrOfFrames;
			m_AnimTime -= 1.f / m_NrFramesPerSec;
		}
	}
}

void MiniBoss::ChangeShape()
{
	switch (m_AnimFrame)
	{
	case 0:
		m_Shape.height = 32.f;
		break;
	case 1:
		m_Shape.height = 48.f;
		break;
	case 2:
		m_Shape.height = 60.f;
		break;
	default:
		break;
	}
}
