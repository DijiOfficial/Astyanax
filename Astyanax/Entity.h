#pragma once
#include "Level.h"
#include "FireBall.h"
#include "SoundEffect.h"
#include "Debug.h"

class Entity
{
public:
	const enum EnemyType
	{
		blob,
		skeleton,
		eyeball,
		bomb,
		powerUp,
		plant,
		projectile,
		miniBoss,
	};

	Entity(const Point2f& bottomLeft, const float width, const float horSpeed, const float speed, const int id, const int health, const Texture* texture, const EnemyType enemyType);
	virtual ~Entity();
	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) = 0;
	virtual void Update(float elapsedSec);
	bool IsOverlapping(const Rectf& rect) const;
	Point2f GetPos() const;
	int GetId() const;
	float DistanceFromAvatar(const Rectf& avatar) const;
	Rectf GetAttackZone() const;
	int GetHealth() const;
	void DealDamage(const int damageDealt);
	bool EntityIsHit() const;
	virtual void KillEntity() {};
	EnemyType GetEnemyType() const;
	bool IsHitBySpell(std::vector<FireBall*>& fireballs) const;
	void Freeze();
	bool GetIsFrozen() const;
	bool IsGoingLeft() const;
	int GetEntityId() const;

	const enum ActionState
	{
		spawning,
		alive,
		attacking,
		jumping,
		isHit,
		dying,
		dead,
		idle,
		deactivated
	};
	virtual ActionState GetState() const = 0;

	enum BombState
	{
		Bouncing,
		Exploding,
		Exploded
	};


protected:
	int m_AnimFrame;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	const float m_HorSpeed;
	const float m_JumpSpeed;
	bool m_IsGoingLeft;
	bool m_IsHit;
	float m_AnimTime;
	int m_NrFramesPerSec;
	int m_NrOfFrames;
	Rectf m_Shape;
	Rectf m_AttackZone;
	const Point2f m_Origin;
	const Texture* m_pTexture;//should be m_pTexture
	const Texture* m_pBlastTexture;
	int m_Health;
	bool m_IsFrozen;
	Debug m_Debugger;
private:
	const int m_Id;
	virtual void UpdateFrames(float elapsedSec) = 0;
	EnemyType m_EnemyType;
	float m_FreezeTime;
};

