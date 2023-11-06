#pragma once
#include "Entity.h"

class BossBody final : public Entity
{
public:
	BossBody(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	BossBody(const BossBody& other) = delete;
	BossBody(BossBody&& other) noexcept = delete;
	BossBody& operator=(const BossBody& other) = delete;
	BossBody& operator=(BossBody&& other) noexcept = delete;

	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;

	bool GetCanTurn() const { return m_CanTurn; };
	bool GetIsTurning() const { return m_IsTurning; };
	float GetXVelocity() const { return m_Velocity.x; };
private:
	Rectf m_TextClip;
	ActionState m_ActionState;

	bool m_CanTurn;
	bool m_IsTurning;

	void UpdateFrames(const float elapsedSec) override;
};

