#pragma once
#include "Blob.h"
#include "Skeleton.h"
#include "EyeBallMonster.h"
#include <vector>
#include "structs.h"
#include "HitMarker.h"
#include "Powers.h"
#include "Bombs.h"
#include "Statue.h"
#include "PlantEnemy.h"
#include "Projectile.h"
#include "MiniBoss.h"
#include "FirstBoss.h"

class EntityManager final
{

private:
	std::vector<Entity*> m_pEntity;
	std::vector<HitMarker*> m_pHitMarkers;

	void CheckDeleteEntity(const SoundEffect* deathAudio);
	int m_Score;

public:
	EntityManager();
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = delete;
	EntityManager& operator=(EntityManager&&) = delete;

	void AddEntity(const Point2f& center, const int id, const Entity::EnemyType enemyType, const Texture* texture, const float speed = 0);
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio);
	void Draw() const;
	bool EntityExists(const int id) const;

	void HitEntity(const Rectf& rect, const int damageDealt);
	void CastSpellOnEntity(Powers::Power power, const Rectf& avatar, const int damageDealt, std::vector<FireBall*>& fireballs, const SoundEffect* freezeAudio, const SoundEffect* blastAudio, const SoundEffect* boltAudio) const;
	bool HitAvatar(const Rectf rect) const;
	bool GetStatueState() const;
	Point2f GetStatuePos() const;
	bool GetMiniBossIsDead() const;
	bool GetBossIsDead() const;
	void Reset();

	int GetScore();
};
