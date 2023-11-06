#pragma once
#include "Entity.h"

class Projectile final: public Entity
{
public:
	Projectile(const Point2f& bottomLeft, const int horSpeed, const int speed, const Texture* texture, const Entity::EnemyType enemyType, const int nrOfFrames = 0);
	Projectile(const Projectile& other) = delete;
	Projectile(Projectile&& other) noexcept = delete;
	Projectile& operator=(const Projectile& other) = delete;
	Projectile& operator=(Projectile&& other) noexcept = delete;
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;
private:
	void UpdateFrames(float elapsedSec) override;
	ActionState m_ActionState;
	int m_AnimFrame;
	float m_AnimTime;
	int m_NrOfFrames;
	Rectf m_TextClip;
};

