#pragma once
#include "utils.h"
#include "Level.h"
#include "SoundEffect.h"
#include "Powers.h"
#include "PowerUp.h"
#include "Debug.h"

class Avatar
{
public:
	const enum ActionState
	{
		moving = 1,
		attacking,
		jumping,
		crouched,
		hit,
		castingSpell,
		dying,
		dead,
	};
	Avatar();
	~Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar(Avatar&& other) noexcept = delete;
	Avatar& operator=(const Avatar& other) = delete;
	Avatar& operator=(Avatar&& other) noexcept = delete;
	void Update(float elapsedSec, Level*& level, SoundEffect* swingSwoosh, bool isPlayerLocked);
	void Draw() const;
	void PowerUpHit(PowerUp::PowerType power);
	Rectf GetShape() const;
	
	int GetHealth() const;
	int GetDamageDealt() const;
	int GetSpellDamage() const;
	int GetStrength() const;
	int GetMana() const;
	bool IsAttacking() const;
	bool IsCastingSpell() const;
	Rectf GetAttackZone() const;
	void DealOneDamage(SoundEffect* playerHurt);
	void ChangePower();
	Powers::Power GetCurrentPower() const;
	std::vector<FireBall*>& GetFireBallVector() const;
	void ForceMoveRight();
	int GetMaxStrength() const;
	void SetWeaponChoice(const int choice);
	ActionState GetState() const;;
	void Reset();
private:
	Rectf m_Shape;
	const float m_HorSpeed;
	const float m_JumpSpeed;
	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	

	ActionState m_ActionState;
	int m_AddOne;
	int m_Power;
	int m_Health;
	int m_Mana;
	int m_Strength;
	int m_CurrentMaxStrength;
	Powers* m_CurrentPower;
	float m_SpeedBuff;

	const Texture* m_pSpritesTexture; //const
	const Texture m_DeathTexture;
	float m_ClipHeight;
	float m_ClipWidth;
	int m_NrOfFrames;
	int m_NrFramesPerSec;
	float m_AnimTime;
	float m_InvincibilityTime;
	float m_StrengthResetTime;
	int m_AnimFrame;
	bool m_IsAttacking;
	bool m_IsWaiting;
	bool m_IsGoingLeft;
	bool m_IsInvincible;
	bool m_ShouldDraw;
	Debug m_Debugger;

	//void CheckStateTransition();
	void CheckStateOnGround(const SoundEffect* swingSwoosh);
	void CheckStateInAir(const float elapsedSec, const SoundEffect* swingSwoosh);
	void MoveAvatar(const float elapsedSec);
	void UpdateFrames();
	void UpdateFramesWalking();
	void UpdateStrength(const float elapsedSec);
	void UpdateInvincibility(const float elapsedSec);
	void ReduceMana();
	bool HasEnoughMana() const;
};

