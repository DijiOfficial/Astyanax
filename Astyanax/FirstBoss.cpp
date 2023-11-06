#include "pch.h"
#include "FirstBoss.h"
#include "BossBody.h"
#include "BossHead.h"
#include <iostream>

FirstBoss::FirstBoss(const Point2f& bottomLeft, const int id, const Entity::EnemyType enemyType, const Texture* texture)
	: Entity(bottomLeft, 1, 100, 1, id, 550, texture, enemyType)
	, m_BottomLeft{ bottomLeft }
	, m_ActionState{ Entity::ActionState::spawning }
	, m_SwitchPhase{ true }
	, m_PlayFirstPhaseDeathAnimation{ true }
	, m_TextClip{ 0, 0, 63, 56 }
	, m_IdleTime{ 0 }
	, m_IsAttacking{ false }
	, m_AttackTime{ 0 }
{
	m_BossBody = new BossBody(bottomLeft, abs(int("BossBody")), new Texture("bossBody.png"), Entity::EnemyType::boss);
	m_BossHead = new BossHead(Point2f{ bottomLeft.x - 35, bottomLeft.y + 135 }, abs(int("BossHead")), new Texture("bossHead.png"), Entity::EnemyType::boss);
}

FirstBoss::~FirstBoss()
{
	delete m_BossBody;
	m_BossBody = nullptr;
	delete m_BossHead;
	m_BossHead = nullptr;
}

void FirstBoss::Draw() const
{
	if (m_PlayFirstPhaseDeathAnimation)
		m_BossBody->Draw();

	if (m_Health > 225)
		m_BossHead->Draw();
	else
		DrawMainBody();
}

void FirstBoss::Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);

	if (m_Health > 225)
		FirstPhase(elapsedSec, level, avatar, deathAudio);
	else
	{
		if (m_SwitchPhase)
		{
			KillFirstPhaseEntities();
			CreateSecondPhase();
			m_SwitchPhase = false;
			deathAudio->SetVolume(256);
			deathAudio->Play(0);
		}

		if (m_PlayFirstPhaseDeathAnimation)
		{
			m_BossBody->Update(elapsedSec, level, avatar, deathAudio);
			if (m_BossBody->GetState() == ActionState::dead)
				m_PlayFirstPhaseDeathAnimation = false;
		}
		
		SecondPhase(elapsedSec, level, avatar, deathAudio);
	}
}

void FirstBoss::KillEntity()
{
	if (m_ActionState != ActionState::isHit and m_ActionState != ActionState::dead)
	{
		m_ActionState = ActionState::dying;
	}
}

Entity::ActionState FirstBoss::GetState() const
{
	return m_ActionState;
}

void FirstBoss::CheckForDesync()
{
	const float XPosBody{ m_BossBody->GetAttackZone().left };
	const float XPosHead{ m_BossHead->GetAttackZone().left };

	if (XPosBody < XPosHead + 34 or XPosBody > XPosHead + 36)
	{
		m_BossHead->SetXPosition(XPosBody - 35);
		m_BossHead->SetXVelocity(m_BossBody->GetXVelocity());
	}
}

void FirstBoss::FirstPhase(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	if (m_IsHit)
		m_BossHead->SetIsHit(true);

	if (m_IsFrozen)
	{
		m_BossHead->Update(elapsedSec, level, avatar, deathAudio);

		if (not m_BossHead->GetIsHit())
			m_IsHit = false;

		return;
	}

	//1frame delay created because I need a variable from m_BossBody to check if the boss is turning
	//so take into account this frame delay when updating the head make sure it doesnt move if the boss isnt
	if (not m_BossHead->GetIsAttacking())
		m_BossBody->Update(elapsedSec, level, avatar, deathAudio);

	m_BossHead->Update(elapsedSec, level, avatar, deathAudio, m_BossBody->GetIsTurning());

	//check for desync between head and body
	if (m_BossHead->GetState() != ActionState::jumping)
		CheckForDesync();

	if (not m_BossHead->GetIsHit())
		m_IsHit = false;


	m_ActionState = m_BossHead->GetState();
	m_Shape = m_BossHead->GetAttackZone();

	if (m_ActionState == ActionState::jumping)
		m_AttackZone = m_Shape;
	else
		m_AttackZone = m_BossBody->GetAttackZone();
}

void FirstBoss::SecondPhase(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	if (m_ActionState == ActionState::attacking)
		m_ActionState = ActionState::alive;

	if (m_AttackTime >= 0.5f)
	{
		m_IsAttacking = false;
		m_AttackTime = 0;
	}

	if (m_IsHit and m_ActionState != ActionState::isHit and not m_IsFrozen)
	{
		if (m_IsGoingLeft) m_Velocity.x = 180;
		else m_Velocity.x = -180;
		m_Velocity.y = 300;
		m_ActionState = ActionState::isHit;
	}
	if (m_ActionState != ActionState::dying and m_ActionState != ActionState::dead)
	{
		if (m_ActionState != ActionState::isHit and not m_IsFrozen and not m_IsAttacking)
		{
			if (m_Shape.left >= avatar.left) m_IsGoingLeft = true;
			else m_IsGoingLeft = false;
			if (DistanceFromAvatar(avatar) >= (avatar.width * 2 + m_Shape.width * 4))
			{
				m_IdleTime += elapsedSec;

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

				if (m_IdleTime >= 0.3f)
				{
					if (rand() % 2 == 0)
					{
						m_ActionState = ActionState::attacking;
						m_IsAttacking = true;
					}

					m_IdleTime -= 0.3f;
				}
			}
			else m_ActionState = ActionState::slash;
		}
		else
		{
			if (level->IsOnGround(m_Shape, m_Velocity) and m_Velocity.y <= 0) //making sur reset only when coming down not going up
			{
				if (not m_IsFrozen) m_ActionState = ActionState::alive;
				m_IsHit = false;
				m_AnimFrame = 0;
			}
		}
	}

	if (not m_IsFrozen or m_ActionState == ActionState::dying or m_ActionState == ActionState::dead)
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
	}
	
	float attackRange{ 66 };
	if (m_AnimFrame == 0)
		attackRange = 50;

	if (m_IsGoingLeft) 
		m_AttackZone = Rectf{ m_Shape.left - attackRange, m_Shape.bottom + m_Shape.height, attackRange, m_Shape.height };
	else 
		m_AttackZone = Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom + m_Shape.height, attackRange, m_Shape.height };

	if (m_ActionState != ActionState::slash)
		m_AttackZone = m_Shape;

	level->HandleCollision(m_Shape, m_Velocity);
	UpdateFrames(elapsedSec);

	if (m_IsAttacking)
		m_AttackTime += elapsedSec;
}

void FirstBoss::KillFirstPhaseEntities() const
{
	m_BossBody->KillEntity();
}

void FirstBoss::CreateSecondPhase()
{
	m_ActionState = ActionState::alive;
	Rectf bossBodyShape{ m_BossBody->GetAttackZone() };
	m_Shape = Rectf{ bossBodyShape.left + bossBodyShape.width / 2, bossBodyShape.bottom + bossBodyShape.height / 3, 30, 48 };
	m_AttackZone = m_Shape;

	//jump when first phase is over
	m_Velocity.y = 500;
}

void FirstBoss::DrawMainBody() const
{
	Rectf dstRect2, dstRect3;
	Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	
	if (m_IsAttacking) 
		srcRect = Rectf{ m_TextClip.left + 2 * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };
	
	if (m_ActionState == ActionState::slash)
		srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 3) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };

	if (m_ActionState == ActionState::isHit)
		srcRect = Rectf{ m_TextClip.left + (m_AnimFrame + 5) * m_TextClip.width , m_TextClip.bottom, m_TextClip.width, m_TextClip.height };

	
	Rectf dstRect{ -66.f, 0.f , m_TextClip.width * 2, m_TextClip.height * 2 };

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
		if (not m_IsGoingLeft)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_Shape.left - m_Shape.width * 2, m_Shape.bottom, 0);
		}
		else
			glTranslatef(m_Shape.left, m_Shape.bottom, 0);

		if (m_ActionState != ActionState::dying and m_ActionState != ActionState::dead) m_pTexture->Draw(dstRect, srcRect);
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
		float attackRange{ 66 };
		if (m_AnimFrame == 0)
			attackRange = 50;

		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	
		if (m_ActionState != ActionState::slash)
			return;

		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		if (m_IsGoingLeft) utils::DrawRect(Rectf{ m_Shape.left - attackRange, m_Shape.bottom + m_Shape.height, attackRange , m_Shape.height });
		else utils::DrawRect(Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom + m_Shape.height, attackRange, m_Shape.height });
	}
}

void FirstBoss::UpdateFrames(const float elapsedSec)
{
	if (not m_IsFrozen or m_ActionState == dying) 
		m_AnimTime += elapsedSec;

	if (m_ActionState == ActionState::isHit)
	{
		if (m_AnimTime >= 0.2f / (m_NrFramesPerSec + 1))
		{
			++m_AnimFrame %= (m_NrOfFrames + 1);
			m_AnimTime -= 0.2f / (m_NrFramesPerSec + 1);
		}
	}
	else if (m_ActionState == ActionState::dying)
	{
		if (m_AnimTime >= 0.8f / (m_NrFramesPerSec + 8))
		{
			++m_AnimFrame;
			m_AnimTime -= 0.8f / (m_NrFramesPerSec + 8);
		}
		if (m_AnimFrame >= 9) m_ActionState = ActionState::dead;
	}
	else if (not m_IsFrozen)
	{
		if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
		{
			++m_AnimFrame %= m_NrOfFrames;
			m_AnimTime -= 0.5f / m_NrFramesPerSec;
		}
	}
}
