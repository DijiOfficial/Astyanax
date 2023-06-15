// ReSharper disable All
#include "pch.h"
#include "Avatar.h"
#include <iostream>

Avatar::Avatar() :
	m_Shape{ Rectf{100, 280, 28, 48} },
	m_HorSpeed{ 100.f },
	m_JumpSpeed{ 800.f },
	m_Velocity{ Vector2f{0.0f, 0.0f} },
	m_Acceleration{ Vector2f{ 0, -981.0f } },

	m_ActionState{ ActionState::moving },
	m_AddOne{ 1 },
	m_Power{ 0 },
	m_Health{ 20 },
	m_Mana{ 20 },
	m_Strength{ 5 },
	m_CurrentMaxStrength{ 5 },
	m_CurrentPower{ new Powers(Powers::Power::blast)},
	m_SpeedBuff{ 0.f },

	m_pSpritesTexture{ new Texture {"PlayerSpriteSheet.png"} },
	m_DeathTexture{ Texture{"death.png"} },
	m_ClipHeight{ 70.f },
	m_ClipWidth{ 80.f },
	m_NrOfFrames{ 3 },
	m_NrFramesPerSec{ 3 },
	m_AnimTime{},
	m_InvincibilityTime{},
	m_StrengthResetTime{},
	m_AnimFrame{},
	m_IsAttacking{ false },
	m_IsWaiting{ false },
	m_IsGoingLeft{ false },
	m_IsInvincible{ false },
	m_ShouldDraw{ true }
{

}

Avatar::~Avatar()
{
	delete m_pSpritesTexture;
	delete m_CurrentPower;
}

void Avatar::Draw() const
{
	Rectf srcRect{}, dstRect{};
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	srcRect.left = (m_ClipWidth * 3) * m_Power + m_AnimFrame * srcRect.width;
	srcRect.bottom = m_ClipHeight * int(m_ActionState);
	
	dstRect = Rectf{ 0.f, 0.f , m_ClipWidth * 2, m_ClipHeight * 2 };
	if (m_ActionState == ActionState::jumping or m_ActionState == ActionState::attacking)
	{
		dstRect = Rectf{ -40.f, 0.f , m_ClipWidth * 2, m_ClipHeight * 2 };
	}
	if (m_ActionState == ActionState::crouched)
	{
		dstRect = Rectf{ -34, 0.f , m_ClipWidth * 2, m_ClipHeight * 2 };
	}

	glPushMatrix();
	{
		if (m_IsGoingLeft)
		{
			glScalef(-1,1,1);
			glTranslatef(-m_Shape.left - m_Shape.width * 2, m_Shape.bottom, 0);
		}
		else glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		if (m_ShouldDraw and m_ActionState != ActionState::dying) m_pSpritesTexture->Draw(dstRect, srcRect);
		else if (m_ActionState == ActionState::dying)
		{
			srcRect = Rectf{ m_AnimFrame * m_ClipWidth, 0 , m_ClipWidth, m_ClipHeight };
			dstRect = Rectf{ 0.f, 0.f , m_ClipWidth * 2, m_ClipHeight * 2 };
			m_DeathTexture.Draw(dstRect, srcRect);
		}
	}
	glPopMatrix();
	m_CurrentPower->Draw();
	if (m_Debugger.m_ShowHitBox) { //collision boxes
		float attackRange;
		const float verticalRange{ m_Shape.height };
		if (m_ActionState == ActionState::crouched or m_ActionState == ActionState::jumping)
		{
			attackRange = { 50 };
		}
		else
		{
			attackRange = { 63 };
		}
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		if (m_ActionState == ActionState::crouched)
		{
			if (m_IsGoingLeft) utils::DrawRect(Rectf{ m_Shape.left - attackRange, m_Shape.bottom, attackRange , verticalRange });
			else utils::DrawRect(Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom, attackRange, verticalRange });
		}
		else
		{
			if (m_IsGoingLeft) utils::DrawRect(Rectf{ m_Shape.left - attackRange, m_Shape.bottom + verticalRange , attackRange , verticalRange });
			else utils::DrawRect(Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom + verticalRange, attackRange, verticalRange });
		}
	}

}

void Avatar::PowerUpHit(const PowerUp::PowerType power)
{
	switch (power)
	{
	case PowerUp::Strength:
		m_CurrentMaxStrength += 5;
		break;
	case PowerUp::Speed:
		m_SpeedBuff += 0.025f;
		break;
	case PowerUp::Health:
		m_Health = 20;
		break;
	case PowerUp::Magic:
		m_Mana = 20;
		break;
	default:
		break;
	}
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}


void Avatar::Update(const float elapsedSec, Level*& level, SoundEffect* swingSwoosh, const bool isPlayerLocked)
{
	if (m_IsInvincible) UpdateInvincibility(elapsedSec);
	else m_ShouldDraw = true;
	if (m_ActionState != ActionState::dying and m_ActionState != ActionState::dead)
	{
		if (m_Strength < m_CurrentMaxStrength)
		{
			UpdateStrength(elapsedSec);
		}
		if (m_ActionState == ActionState::hit)
		{
			m_IsAttacking = false;
			MoveAvatar(elapsedSec);
		}
		if (m_IsAttacking and m_Strength == m_CurrentMaxStrength and m_AnimFrame != 2)
		{
			m_Strength = 0;
		}

		if (level->IsOnGround(m_Shape, m_Velocity))
		{
			if (m_ActionState != ActionState::crouched or (m_ActionState != ActionState::attacking and m_ActionState != ActionState::crouched)) m_IsAttacking = false;
			if (m_ActionState == ActionState::jumping) 
			{
				m_ActionState = ActionState::moving;
				m_IsAttacking = false;
				m_AddOne = abs(m_AddOne);
				m_AnimFrame = 0;
			}
			if (not isPlayerLocked) CheckStateOnGround(swingSwoosh);
		}
		else if (m_ActionState != ActionState::hit and not isPlayerLocked)
	{
		CheckStateInAir(elapsedSec, swingSwoosh);
	}
	}

	if (not m_IsWaiting or m_ActionState == ActionState::attacking or (m_ActionState == ActionState::dying))
	{
		m_AnimTime += elapsedSec;
		if (m_ActionState != ActionState::hit) MoveAvatar(elapsedSec);
		if (m_ActionState == ActionState::moving)
			UpdateFramesWalking();
		else 
			UpdateFrames();
	}

	level->HandleCollision(m_Shape, m_Velocity);
	m_CurrentPower->Update(elapsedSec);
	if (m_Shape.bottom <= -50) m_ActionState = ActionState::dead;
}


void Avatar::CheckStateOnGround(const SoundEffect* swingSwoosh)
{
	const bool isMovementNotBlocked{ m_ActionState != ActionState::crouched and m_ActionState != ActionState::attacking and m_ActionState != ActionState::castingSpell and not m_IsAttacking };
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	const bool isMoving{ pStates[SDL_SCANCODE_LEFT] or pStates[SDL_SCANCODE_RIGHT] or pStates[SDL_SCANCODE_UP] };

	if (pStates[SDL_SCANCODE_DOWN] and isMovementNotBlocked)
	{
		m_ActionState = ActionState::crouched;
		m_Shape.height = 32.f;

		m_AnimTime = 0;
		m_AnimFrame = 0;
	}
	else if (pStates[SDL_SCANCODE_DOWN] and m_ActionState == ActionState::crouched)
	{
		m_ActionState = ActionState::crouched;
	}
	else if (m_ActionState == ActionState::crouched and not m_IsAttacking) //because crouching is not time based, need reset sprite when key not pressed
	{
		m_ActionState = ActionState::moving;
		m_AnimTime = 0;
		m_AnimFrame = 0;
		m_AddOne = abs(m_AddOne);
	}
	else
	{
		m_Shape.height = 48.f;
	}

	if (pStates[SDL_SCANCODE_LEFT] and isMovementNotBlocked)
	{
		if(m_ActionState != ActionState::attacking and m_ActionState != ActionState::crouched) m_ActionState = ActionState::moving;
		m_Velocity.x = -m_HorSpeed;
		m_IsGoingLeft = true;
	}
	else if (pStates[SDL_SCANCODE_LEFT] and m_ActionState == ActionState::crouched) m_IsGoingLeft = true;
	
	if (pStates[SDL_SCANCODE_RIGHT] and isMovementNotBlocked)
	{
		if (m_ActionState != ActionState::attacking and m_ActionState != ActionState::crouched) m_ActionState = ActionState::moving;
		m_Velocity.x = m_HorSpeed;
		m_IsGoingLeft = false;
	}
	else if (pStates[SDL_SCANCODE_RIGHT] and m_ActionState == ActionState::crouched) m_IsGoingLeft = false;
	
	if (pStates[SDL_SCANCODE_UP] and isMovementNotBlocked)
	{
		m_ActionState = ActionState::jumping;
		m_Velocity.y = m_JumpSpeed / 2;

		m_AnimTime = 0;
		m_AnimFrame = 0;
	}
	if (pStates[SDL_SCANCODE_X] and isMovementNotBlocked)
	{
		m_AnimTime = 0;
		m_AnimFrame = 0;
		m_ActionState = ActionState::attacking;
		m_IsAttacking = true;
		swingSwoosh->Play(0);
	}
	if (pStates[SDL_SCANCODE_X] and m_ActionState == ActionState::crouched and not m_IsAttacking)
	{
		m_IsAttacking = true;
		swingSwoosh->Play(0);
	}

	if (pStates[SDL_SCANCODE_C] and isMovementNotBlocked and HasEnoughMana())
	{
		m_AnimTime = 0;
		m_AnimFrame = 0;
		m_ActionState = ActionState::castingSpell;
		if (m_CurrentPower->GetCurrentPower() == Powers::Power::blast)
		{
			m_CurrentPower->AddFireballs(m_Shape);
		}
		ReduceMana();
	}

	if (!pStates[SDL_SCANCODE_LEFT] && !pStates[SDL_SCANCODE_C] && m_ActionState != ActionState::castingSpell and not m_IsAttacking and !pStates[SDL_SCANCODE_RIGHT] && !pStates[SDL_SCANCODE_UP] and not pStates[SDL_SCANCODE_DOWN] and not pStates[SDL_SCANCODE_X])
		m_IsWaiting = true;
	else m_IsWaiting = false;
}

void Avatar::CheckStateInAir(const float elapsedSec, const SoundEffect* swingSwoosh)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x = -m_HorSpeed;
		m_IsGoingLeft = true;
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x = m_HorSpeed;
		m_IsGoingLeft = false;
	}
	if (pStates[SDL_SCANCODE_X] and not m_IsAttacking)
	{
		m_IsAttacking = true;
		swingSwoosh->Play(0);

	}
	if (pStates[SDL_SCANCODE_UP])
	{
		if (m_Velocity.y > 0)
		{
			m_Velocity.y += m_JumpSpeed * elapsedSec / 2;
		}
	}
}

void Avatar::MoveAvatar(const float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
}

void Avatar::UpdateFramesWalking() 
{
	if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
	{
		m_AnimFrame += m_AddOne;
		if (m_AnimFrame <= 0 or m_AnimFrame >= 2) m_AddOne = -m_AddOne;
		m_AnimTime -= 0.5f / m_NrFramesPerSec;
	}
}

void Avatar::UpdateFrames()
{
	if (m_AnimTime >= 0.5f / m_NrFramesPerSec)
	{
		++m_AnimFrame %= m_NrOfFrames;
		//Full animation for attacking
		if (m_ActionState == ActionState::attacking and m_AnimFrame == 0)
		{
			m_ActionState = ActionState::moving;
			m_AddOne = abs(m_AddOne);
		}
		//Block animation for jump attack
		if (m_ActionState == ActionState::jumping)
		{
			if (not m_IsAttacking) m_AnimFrame -= 1;// check if reseting on ground needs things removed 
			if (m_IsAttacking and m_AnimFrame == 0) m_AnimFrame = 2;
		}
		if (m_ActionState == ActionState::crouched)
		{
			if (not m_IsAttacking) m_AnimFrame -= 1;// check if reseting on ground needs things removed 
			if (m_IsAttacking and m_AnimFrame == 0)
			{
				m_IsAttacking = false;
			}
		}
		if (m_ActionState == ActionState::castingSpell )
		{
			m_AnimTime -= 0.5f / m_NrFramesPerSec;
			if (m_AnimFrame == 0)
			{
				m_ActionState = ActionState::moving;
				m_AnimTime = 0;
				m_AnimFrame = 0;
				m_AddOne = abs(m_AddOne);
			}
		}
		if (m_ActionState == ActionState::dying and m_AnimFrame == 0)
		{
			m_ActionState = ActionState::dead;
		}
		m_AnimTime -= 0.5f / m_NrFramesPerSec;
	}
}

void Avatar::UpdateStrength(const float elapsedSec)
{
	m_StrengthResetTime += elapsedSec;
	if (m_StrengthResetTime >= 0.15f - m_SpeedBuff)
	{
		m_Strength += 1;
		m_StrengthResetTime -= 0.15f - m_SpeedBuff;
	}
}

void Avatar::UpdateInvincibility(const float elapsedSec)
{
	if (static_cast<int>(m_InvincibilityTime * 50) % 2 == 0) m_ShouldDraw = !m_ShouldDraw;

	m_InvincibilityTime += elapsedSec;
	if (m_InvincibilityTime >= 3.f)
	{
		m_IsInvincible = false;
		m_InvincibilityTime = 0.f;
	}
}

Powers::Power Avatar::GetCurrentPower() const
{
	return m_CurrentPower->GetCurrentPower();
}

int Avatar::GetHealth() const
{
	return m_Health;
}

int Avatar::GetDamageDealt() const
{
	if (m_Debugger.m_OnePunch) return 999999999;
	int damageDealt{};
	if (m_ActionState == ActionState::jumping) damageDealt = m_CurrentMaxStrength;
	else damageDealt = m_Strength;
	
	switch (m_Power)
	{
	case 0:
		return damageDealt * 2;
		break;
	case 1:
		return damageDealt;
		break;
	case 2:
		return damageDealt * 3;
		break;
	default:
		break;
	}
}

int Avatar::GetStrength() const
{
	return m_Strength;
}

int Avatar::GetMana() const
{
	return m_Mana;
}

bool Avatar::IsAttacking() const
{
	return m_IsAttacking;
}

bool Avatar::IsCastingSpell() const
{
	return m_ActionState == ActionState::castingSpell;
}

Rectf Avatar::GetAttackZone() const
{
	float attackRange;
	float verticalRange{ m_Shape.height / 2};
	if (m_ActionState == ActionState::crouched or m_ActionState == ActionState::jumping)
		attackRange = { 25 };
	else
		attackRange = { 32 };
	if (m_ActionState == ActionState::crouched)
	{
		if (m_IsGoingLeft) return (Rectf{ m_Shape.left - attackRange * 2, m_Shape.bottom, attackRange , verticalRange });
		else return (Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom, attackRange, verticalRange });
	}
	else
	{
		if (m_IsGoingLeft) return (Rectf{ m_Shape.left - attackRange * 2, m_Shape.bottom + m_Shape.height , attackRange , verticalRange });
		else return (Rectf{ m_Shape.left + m_Shape.width * 2, m_Shape.bottom + m_Shape.height, attackRange, verticalRange });
	}
}

void Avatar::DealOneDamage(SoundEffect* playerHurt)
{
	if (not m_IsInvincible)
	{
		m_Health -= 1;
		m_ActionState = ActionState::hit;
		if (m_IsGoingLeft) m_Velocity.x = 100;
		else m_Velocity.x = -100;
		m_Velocity.y = 200;
		m_IsWaiting = false; // because not pressing keys is considered waiting so if hit when not hitting keys doesnt update animation frames
		playerHurt->Play(0);
		if (m_Health <= 0)
		{
			m_ActionState = ActionState::dying;
			m_NrFramesPerSec = 1;
			m_AnimTime;
			m_IsInvincible = true;
			m_NrOfFrames = 2;
			m_ClipWidth = 56;
			m_ClipHeight = 30;
			m_Shape.height = 30;
			m_Shape.width = 56;
		}
	}
	m_IsInvincible = true;
}

void Avatar::ChangePower()
{
	m_CurrentPower->ChangePower();
}

void Avatar::ReduceMana()
{
	int spellConsumption{};
	
	if (m_Power == 0) spellConsumption = 2;
	else if (m_Power == 1) spellConsumption = 1;
	else if (m_Power == 2) spellConsumption = 3;

	switch (m_CurrentPower->GetCurrentPower())
	{
	case Powers::bolt:
		m_Mana -= 5.f * spellConsumption;
		break;
	case Powers::blast:
		m_Mana -= 3.f * spellConsumption;
		break;
	case Powers::bind:
		m_Mana -= 1.f * spellConsumption;
		break;
	default:
		break;
	}
	if (m_Debugger.m_InfinitPower) m_Mana = 20;
}

bool Avatar::HasEnoughMana() const
{
	int spellConsumption{};

	if (m_Power == 0) spellConsumption = 2;
	else if (m_Power == 1) spellConsumption = 1;
	else if (m_Power == 2) spellConsumption = 3;

	switch (m_CurrentPower->GetCurrentPower())
	{
	case Powers::bolt:
		return m_Mana >= 5.f * spellConsumption;
		break;
	case Powers::blast:
		return m_Mana >= 3.f * spellConsumption;
		break;
	case Powers::bind:
		return m_Mana >= 1.f * spellConsumption;
		break;
	default:
		break;
	}
}

int Avatar::GetSpellDamage() const
{
	int spellDamage{};

	switch (m_CurrentPower->GetCurrentPower()) // no idea about those values
	{
	case Powers::bolt:
		spellDamage = 45;
		break;
	case Powers::blast:
		spellDamage = 25;
		break;
	case Powers::bind:
		spellDamage = 0;
		break;
	default:
		break;
	}

	switch (m_Power)
	{
	case 0:
		return  spellDamage * 2;
		break;
	case 1:
		return spellDamage * 3;
		break;
	case 2:
		return spellDamage * 1;
		break;
	default:
		break;
	}
}

std::vector<FireBall*>& Avatar::GetFireBallVector() const
{
	return m_CurrentPower->GetFireBallVector();
}

void Avatar::ForceMoveRight()
{
	m_IsGoingLeft = false;
	m_Shape.left += 1;
}

int Avatar::GetMaxStrength() const
{
	return m_CurrentMaxStrength;
}

void Avatar::SetWeaponChoice(const int choice)
{
	m_Power = choice;
}

Avatar::ActionState Avatar::GetState() const
{ return m_ActionState; }

void Avatar::Reset()
{
	m_Shape = { Rectf{100, 280, 28, 48} };
	m_Velocity = { Vector2f{0.0f, 0.0f} };

	m_ActionState = { ActionState::moving };
	m_Health={ 20 };
	m_Mana={ 20 };
	m_ClipHeight={ 70.f };
	m_ClipWidth={ 80.f };
	m_NrOfFrames={ 3 };
	m_NrFramesPerSec={ 3 };
	m_AnimTime={},
	m_StrengthResetTime={},
	m_AnimFrame={},
	m_IsWaiting={ false };
	m_IsAttacking={ false };
	m_IsGoingLeft={ false };
	m_InvincibilityTime={},
	m_IsInvincible={ false };
	m_ShouldDraw = { true };
}
