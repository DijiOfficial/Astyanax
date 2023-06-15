#pragma once
#include "Entity.h"

class Statue final : public Entity
{
public:
	Statue(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	Statue(const Statue& other) = delete;
	Statue(Statue&& other) noexcept = delete;
	Statue& operator=(const Statue& other) = delete;
	Statue& operator=(Statue&& other) noexcept = delete;
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;
private:
	ActionState m_ActionState;
	void UpdateFrames(float elapsedSec) override;
	float m_HitTimer;
};

