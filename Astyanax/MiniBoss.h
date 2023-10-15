#pragma once
#include "Entity.h"

class MiniBoss final : public Entity
{
public:
	MiniBoss(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	MiniBoss(const MiniBoss& other) = delete;
	MiniBoss(MiniBoss&& other) noexcept = delete;
	MiniBoss& operator=(const MiniBoss& other) = delete;
	MiniBoss& operator=(MiniBoss&& other) noexcept = delete;
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;
private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	float m_HitAnimTime;
	float m_TotalHitAnimTime;
	void UpdateFrames(float elapsedSec) override;
	void ChangeShape();
	const float m_OriginalHeight;
	bool m_CanBounce;
	int m_Bounces;
	Point2f m_Origin;
	Point2f m_CollisionPoint;
	float m_IdleTime;
	float m_TotalIdleTime;
};

