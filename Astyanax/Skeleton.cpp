#include "pch.h"
#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 32.f, 100.f, -120.f, id, 25, texture, enemyType),
	m_ActionState{ spawning }
{
	m_TextClip = { Rectf{ 0.f, 0.f, 60.f, m_pTexture->GetHeight()} };
}

void Skeleton::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);
	if (m_IsHit and m_ActionState != isHit and not m_IsFrozen)
	{
		if (m_IsGoingLeft) m_Velocity.x = 180;
		else m_Velocity.x = -180;
		m_Velocity.y = 300;
		m_ActionState = isHit;
	}
	if (m_ActionState != dying and m_ActionState != dead)
	{
		if (m_ActionState != spawning and m_ActionState != isHit and not m_IsFrozen)
		{
			if (m_Shape.left >= avatar.left) m_IsGoingLeft = true;
			else m_IsGoingLeft = false;
			if (DistanceFromAvatar(avatar) >= (avatar.width * 2 + m_Shape.width * 4))
			{
				if (level->IsOnGround(m_Shape, m_Velocity)) m_ActionState = alive;
				if (m_Shape.left >= avatar.left)
				{
					m_Velocity.x = -m_HorSpeed;
					m_IsGoingLeft = true;
				}
				else
				{
					m_Velocity.x = m_HorSpeed;
					m_IsGoingLeft = false;
				}
			
				if (level->IsCollidingWithWorld(m_Shape))
				{
					m_ActionState = jumping;
					if (m_Velocity.y > 0) m_Velocity.y += 800 * elapsedSec;
					else m_Velocity.y = 550;
				} 
			}
			else m_ActionState = attacking;
		}
		else if (m_ActionState != spawning)
		{
			if (level->IsOnGround(m_Shape, m_Velocity) and m_Velocity.y <= 0) //making sur reset only when coming down not going up
			{
				if (not m_IsFrozen) m_ActionState = alive;
				m_IsHit = false;
			}
		}
	}

	if (not m_IsFrozen or m_ActionState == dying or m_ActionState == dead)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
	}

	constexpr float attackRange{ 40 };
	if (m_IsGoingLeft) m_AttackZone = Rectf{ m_Shape.left - attackRange, m_Shape.bottom, attackRange, m_Shape.height * 2 };
	else m_AttackZone = Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom, attackRange, m_Shape.height * 2};

	level->HandleCollision(m_Shape, m_Velocity);
	UpdateFrames(elapsedSec);

}

void Skeleton::Draw() const
{
	//switch ms5tate
	Rectf dstRect2, dstRect3;
	Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };//replace with action state values 1 line instead of 3
	if (m_ActionState == alive) srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 2) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	if (m_ActionState == attacking) srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 4) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	if (m_ActionState == jumping) srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 3) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	if (m_ActionState == isHit) srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 6) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	Rectf dstRect{ -36, 0.f , m_TextClip.width * 2, m_TextClip.height * 2 };
	
	if (m_ActionState == dying)
	{
		srcRect = Rectf{ m_AnimFrame * m_TextClip.width * 2 , m_TextClip.bottom + m_TextClip.width * 2, m_TextClip.width * 2, m_TextClip.width * 2};
		if (m_AnimFrame >= 5)
		{
			srcRect = Rectf{ m_AnimFrame * m_TextClip.width * 2 , m_TextClip.bottom, m_TextClip.width * 2, m_TextClip.width * 2 };
		}
		dstRect  = Rectf{ -10.f,						0.f ,								4 * m_TextClip.width / 5, 4 * m_TextClip.width / 5 };
		dstRect2 = Rectf{ -36.f + m_TextClip.width,		7.f ,								4 * m_TextClip.width / 5, 4 * m_TextClip.width / 5 };
		dstRect3 = Rectf{ -23.f + m_TextClip.width / 2, -23.f + 2 * m_TextClip.height / 3 , 4 * m_TextClip.width / 5, 4 * m_TextClip.width / 5 };
	}
	glPushMatrix();
	{
		if (not m_IsGoingLeft)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_Shape.left - m_Shape.width * 2, m_Shape.bottom, 0);
		}
		else glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		if (m_ActionState != dying and m_ActionState != dead) m_pTexture->Draw(dstRect, srcRect);
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
		constexpr float attackRange{ 40 };
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		if (m_IsGoingLeft) utils::DrawRect(Rectf{ m_Shape.left - attackRange, m_Shape.bottom, attackRange , m_Shape.height * 2});
		else utils::DrawRect(Rectf{ m_Shape.left + m_Shape.width*2, m_Shape.bottom, attackRange, m_Shape.height * 2 });
	}
}

void Skeleton::UpdateFrames(const float elapsedSec)
{
	if (not m_IsFrozen or m_ActionState == dying) m_AnimTime += elapsedSec;
	if (m_ActionState == isHit)
	{
		if (m_AnimTime >= 0.5f / (m_NrFramesPerSec + 1))
		{
			++m_AnimFrame %= (m_NrOfFrames + 1);
			m_AnimTime -= 0.5f / (m_NrFramesPerSec + 1);
		}
	}
	else if (m_ActionState == dying)
	{
		if (m_AnimTime >= 0.8f / (m_NrFramesPerSec + 8))
		{
			++m_AnimFrame;
			m_AnimTime -= 0.8f / (m_NrFramesPerSec + 8);
		}
		if (m_AnimFrame >= 9) m_ActionState = dead;
	}
	else if (not m_IsFrozen)
	{
		if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
		{
			++m_AnimFrame %= m_NrOfFrames;
			if (m_ActionState == spawning and m_AnimFrame == 0) m_ActionState = alive;
			if (m_ActionState == jumping and m_AnimFrame == 0) m_AnimFrame = 1;
			m_AnimTime -= 0.5f / m_NrFramesPerSec;
		}
	}
}

Entity::ActionState Skeleton::GetState() const
{
	return m_ActionState;
}

void Skeleton::KillEntity()
{
	if (m_ActionState != isHit and m_ActionState != dead)
	{
		m_ActionState = dying;
	}
}