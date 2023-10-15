#pragma once
#include "Entity.h"

class Bombs final: public Entity
{
public:
	Bombs(const Point2f& bottomLeft, const int horSpeed, const Texture* texture, const Entity::EnemyType enemyType);
	Bombs(const Bombs& other) = delete;
	Bombs(Bombs&& other) noexcept = delete;
	Bombs& operator=(const Bombs& other) = delete;
	Bombs& operator=(Bombs&& other) noexcept = delete;

	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	void KillEntity() override;
	ActionState GetState() const override;

private:
	const float m_Epsilon;
	int m_Bounces;
	bool m_CanBounce;
	Point2f m_Origin;
	Point2f m_CollisionPoint;
	
	BombState m_BombState;
	void UpdateFrames(float elapsedSec) override;
	BombState GetBombState() const;
};

