#pragma once
#include "Entity.h"

class Blob final: public Entity
{
public:
	Blob(const Point2f& bottomLeft, const int id, const float speed, const Texture* texture, const Entity::EnemyType enemyType);
	Blob(const Blob& other) = delete;
	Blob(Blob&& other) noexcept = delete;
	Blob& operator=(const Blob& other) = delete;
	Blob& operator=(Blob&& other) noexcept = delete;
	void Draw() const override;
	void Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;

private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	void UpdateFrames(const float elapsedSec) override;
};

