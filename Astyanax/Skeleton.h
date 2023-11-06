#pragma once
#include "Entity.h"

class Skeleton final : public Entity
{
public:
	Skeleton(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	Skeleton(const Skeleton& other) = delete;
	Skeleton(Skeleton&& other) noexcept = delete;
	Skeleton& operator=(const Skeleton& other) = delete;
	Skeleton& operator=(Skeleton&& other) noexcept = delete;
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;
private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	void UpdateFrames(float elapsedSec) override;
};
