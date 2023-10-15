#pragma once
#include "Entity.h"

class EyeBallMonster final : public Entity
{
public:
	EyeBallMonster(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	EyeBallMonster(const EyeBallMonster& other) = delete;
	EyeBallMonster(EyeBallMonster&& other) noexcept = delete;
	EyeBallMonster& operator=(const EyeBallMonster& other) = delete;
	EyeBallMonster& operator=(EyeBallMonster&& other) noexcept = delete;
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;

private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	float m_HitAnimTime;
	bool m_IsMovingVertically;
	bool m_LockVerticalMovement;
	float m_NewVerticalPosition;
	void UpdateFrames(float elapsedSec) override;
};

