#pragma once
#include "Entity.h"

class PlantEnemy final : public Entity
{
public:
	PlantEnemy(const Point2f& bottomLeft, const int id, const Texture* texture, const Entity::EnemyType enemyType);
	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;
private:
	Rectf m_TextClip;
	ActionState m_ActionState;
	float m_HitAnimTime;
	float m_TotalHitAnimTime;
	bool m_CanAttack;
	void UpdateFrames(float elapsedSec) override;
};

