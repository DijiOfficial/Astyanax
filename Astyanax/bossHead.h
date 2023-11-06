#pragma once
#include "Entity.h"

class BossHead final : public Entity
{
public:
	BossHead(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	BossHead(const BossHead& other) = delete;
	BossHead(BossHead&& other) noexcept = delete;
	BossHead& operator=(const BossHead& other) = delete;
	BossHead& operator=(BossHead&& other) noexcept = delete;

	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio, const bool isTurning);
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;

	bool GetIsAttacking() const { return m_IsAttacking; };
	void SetIsHit(const bool isHit) { m_IsHit = isHit; };
	bool GetIsHit() const { return m_IsHit; };
	void SetXPosition(const float& xPosition) { m_Shape.left = xPosition; };
	void SetXVelocity(const float& xVelocity) { m_Velocity.x = xVelocity; };
private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	
	float m_IdleTime;
	float m_HitAnimTime;
	float m_TotalHitAnimTime;

	float m_Radius;
	float m_Angle;
	float m_OriginalAngle;
	
	bool m_IsAttacking;
	
	int m_AnimeFrameCount;
	int m_AttackingFrameCount;
	
	Point2f m_CircleCenter;
	Point2f m_LastPosition;
	void UpdateFrames(const float elapsedSec) override;
	void UpdateFrozenFrames(const float elapsedSec);
};


