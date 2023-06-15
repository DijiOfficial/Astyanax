#include "pch.h"
#include "EntityManager.h"
#include <iostream>
EntityManager::EntityManager() :
	m_Score{}
{
}

EntityManager::~EntityManager()
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		delete m_pEntity[i];
		m_pEntity[i] = nullptr;
	}

	for (int i = 0; i < m_pHitMarkers.size(); i++)
	{
		delete m_pHitMarkers[i];
		m_pHitMarkers[i] = nullptr;
	}
}

void EntityManager::AddEntity(const Point2f& center, const int id, const Entity::EnemyType enemyType, const Texture* texture, const float speed)
{
	switch (enemyType)
	{
	case Entity::EnemyType::blob:
		m_pEntity.push_back(new Blob(center, id, speed, texture, enemyType));
		break;
	case Entity::EnemyType::skeleton:
		m_pEntity.push_back(new Skeleton(center, id, texture, enemyType));
		break;
	case Entity::EnemyType::eyeball:
		m_pEntity.push_back(new EyeBallMonster(center, id, texture, enemyType));
		break;
	case Entity::EnemyType::powerUp:
		m_pEntity.push_back(new Statue(center, id, texture, enemyType));
		break;
	case Entity::EnemyType::plant:
		m_pEntity.push_back(new PlantEnemy(center, id, texture, enemyType));
		break;
	case Entity::EnemyType::miniBoss:
		m_pEntity.push_back(new MiniBoss(center, id, texture, enemyType));
		break;
	}
}

bool EntityManager::EntityExists(const int id) const
{
	if (m_pEntity.empty()) return false;
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (m_pEntity[i]->GetId() == id) return true;
	}
	return false;

}

void EntityManager::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	CheckDeleteEntity(deathAudio);

	for (int i = 0; i < m_pHitMarkers.size(); i++)
	{
		if (m_pHitMarkers[i]->GetCurrentAnimFrame() >= 4)
		{
			delete m_pHitMarkers[i];
			m_pHitMarkers[i] = m_pHitMarkers[m_pHitMarkers.size() - 1];
			m_pHitMarkers.pop_back();
		}
	}

	for (int i = 0; i < m_pHitMarkers.size(); i++)
	{
		m_pHitMarkers[i]->Update(elapsedSec);
	}
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		m_pEntity[i]->Update(elapsedSec, level, avatar, deathAudio);
		if (m_pEntity[i]->GetEnemyType() == Entity::eyeball and not m_pEntity[i]->GetIsFrozen())
		{
			if (fabs(m_pEntity[i]->GetPos().x - avatar.left - 80.f) < 1.f or fabs(m_pEntity[i]->GetPos().x - (avatar.left + avatar.width * 2) + 80.f) < 1.f)
			{
				if (rand() % 3 == 0)
				{
					int horSpeed = 30;
					if (m_pEntity[i]->IsGoingLeft()) horSpeed = -horSpeed;
					m_pEntity.push_back(new Bombs(m_pEntity[i]->GetPos(), horSpeed, new Texture{ "Bombs.png" }, Entity::EnemyType::bomb));
				}
			}
		}
		if (m_pEntity[i]->GetEnemyType() == Entity::plant and not m_pEntity[i]->GetIsFrozen())
		{
			if (m_pEntity[i]->GetState() == Entity::ActionState::attacking)
			{	
				int horSpeed = m_pEntity[i]->DistanceFromAvatar(avatar) - avatar.width;
				if (horSpeed > 300) horSpeed = 300;
				const Rectf entityShape{ m_pEntity[i]->GetAttackZone() };
				const Point2f projectilePos{ entityShape.left + entityShape.width / 2, entityShape.bottom + entityShape.height };
				m_pEntity.push_back(new Projectile(projectilePos, horSpeed, 450.f, new Texture{ "Projectile.png" }, Entity::EnemyType::projectile));
				m_pEntity.push_back(new Projectile(projectilePos, -horSpeed, 450.f, new Texture{ "Projectile.png" }, Entity::EnemyType::projectile));
			}
		}
		if (m_pEntity[i]->GetEnemyType() == Entity::miniBoss and not m_pEntity[i]->GetIsFrozen())
		{
			if (m_pEntity[i]->GetState() == Entity::ActionState::attacking)
			{
				constexpr int horSpeed = 300.f;
				const Rectf entityShape{ m_pEntity[i]->GetAttackZone() };
				const Point2f projectilePos{ entityShape.left + entityShape.width / 2, entityShape.bottom + 2 * entityShape.height / 3 };
				m_pEntity.push_back(new Projectile(projectilePos, horSpeed, 0.f, new Texture{ "projectile2.png" }, Entity::EnemyType::projectile));
				m_pEntity.push_back(new Projectile(projectilePos, -horSpeed, 0.f, new Texture{ "projectile2.png" }, Entity::EnemyType::projectile));
			}
		}

	}


	//entities spawns
	{
		if (avatar.left >= 3910 and not EntityExists(abs(int("blob")) + 10) and not EntityExists(abs(int("MiniBoss"))))
		{
			AddEntity(Point2f{ 4100.f, 500.f }, abs(int("MiniBoss")), Entity::EnemyType::miniBoss, new Texture("MiniBoss.png"));
		}
		if (avatar.left <= 260 and not EntityExists(abs(int("blob"))))
		{
			AddEntity(Point2f{ 550.0f, 340.f, }, abs(int("blob")), Entity::EnemyType::blob, new Texture("blob.png"), -120.f);
		}
		if (avatar.left >= 446 and avatar.left <= 448 and not EntityExists(abs(int("skeleton"))))
		{
			AddEntity(Point2f{ 624, 250.f }, abs(int("skeleton")), Entity::EnemyType::skeleton, new Texture("skeletonSpriteSheet.png"));
		}
		if (avatar.left >= 710 and avatar.left <= 720 and not EntityExists(abs(int("EyeBallEnemy"))))
		{
			AddEntity(Point2f{ 900.f, 420.f }, abs(int("EyeBallEnemy")), Entity::EnemyType::eyeball, new Texture("EyeBallEnemy.png"));
		} 
		if (avatar.left >= 740 and avatar.left <= 820 and not EntityExists(abs(int("blob")) + 1))
		{
			AddEntity(Point2f{ 500.f, 340.f }, abs(int("blob")) + 1, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
		}
		if (avatar.left >= 1029 and avatar.left <= 1032 and not EntityExists(abs(int("skeleton")) + 1))
		{
			AddEntity(Point2f{ 1218, 185.f }, abs(int("skeleton")) + 1, Entity::EnemyType::skeleton, new Texture("skeletonSpriteSheet.png"));
		}
		if (avatar.left >= 1218 and avatar.left <= 1222)
		{
			if (not EntityExists(abs(int("blob")) + 2)) AddEntity(Point2f{ 900, 340.f }, abs(int("blob")) + 2, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
			if (not EntityExists(abs(int("EyeBallEnemy")) + 1)) AddEntity(Point2f{ 1410, 400.f }, abs(int("EyeBallEnemy")) + 1, Entity::EnemyType::eyeball, new Texture("EyeBallEnemy.png"));
		}
		if (avatar.left >= 1285 and avatar.left <= 1286 and not EntityExists(abs(int("plantEnemy"))))
		{
			AddEntity(Point2f{ 1411.f, 186.f }, abs(int("plantEnemy")), Entity::EnemyType::plant, new Texture("plantEnemy.png"));
		}
		if (avatar.left >= 1538 and avatar.left <= 1540 and not EntityExists(abs(int("plantEnemy")) + 1))
		{
			AddEntity(Point2f{ 1670.f, 218.f }, abs(int("plantEnemy")) + 1, Entity::EnemyType::plant, new Texture("plantEnemy.png"));
		}
		if (avatar.left >= 1768 and avatar.left <= 1780)
		{
			if (not EntityExists(abs(int("blob")) + 3)) AddEntity(Point2f{ 1450, 320.f }, abs(int("blob")) + 3, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
			if (not EntityExists(abs(int("blob")) + 4)) AddEntity(Point2f{ 2100, 320.f }, abs(int("blob")) + 4, Entity::EnemyType::blob, new Texture("blob.png"), -120.f);
		}
		if (avatar.left >= 2148 and avatar.left <= 2150)
		{
			if (not EntityExists(abs(int("blob")) + 5)) AddEntity(Point2f{ 2470, 240.f }, abs(int("blob")) + 5, Entity::EnemyType::blob, new Texture("blob.png"), -120.f);
			if (not EntityExists(abs(int("plantEnemy")) + 2)) AddEntity(Point2f{ 2278, 122.f }, abs(int("plantEnemy")) + 2, Entity::EnemyType::plant, new Texture("plantEnemy.png"));
		}
		if (avatar.left >= 2528 and avatar.left <= 2530 )
		{
			if (not EntityExists(abs(int("blob")) + 6)) AddEntity(Point2f{ 2210.f, 320.f }, abs(int("blob")) + 6, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
			if (not EntityExists(abs(int("skeleton")) + 2)) AddEntity(Point2f{ 2700.F, 218.f }, abs(int("skeleton")) + 2, Entity::EnemyType::skeleton, new Texture("skeletonSpriteSheet.png"));
		} 
		if (avatar.left >= 2662 and avatar.left <= 2664)
		{
			if (not EntityExists(abs(int("blob")) + 7)) AddEntity(Point2f{ 2340, 280.f }, abs(int("blob")) + 7, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
			if (not EntityExists(abs(int("EyeBallEnemy")) + 2)) AddEntity(Point2f{ 2880, 400.f }, abs(int("EyeBallEnemy")) + 2, Entity::EnemyType::eyeball, new Texture("EyeBallEnemy.png"));
		}
		if (avatar.left >= 2775 and avatar.left <= 2778 and not EntityExists(abs(int("blob")) + 8))
		{
			AddEntity(Point2f{ 3095, 280.f }, abs(int("blob")) + 8, Entity::EnemyType::blob, new Texture("blob.png"), -120.f);
		}
		if (avatar.left >= 3168 and avatar.left <= 3170)
		{
			if (not EntityExists(abs(int("blob")) + 9)) AddEntity(Point2f{ 3490, 280.f }, abs(int("blob")) + 9, Entity::EnemyType::blob, new Texture("blob.png"), -120.f);
			if (not EntityExists(abs(int("EyeBallEnemy")) + 3)) AddEntity(Point2f{ 2920, 400.f }, abs(int("EyeBallEnemy")) + 3, Entity::EnemyType::eyeball, new Texture("EyeBallEnemy.png"));
		}
		if (avatar.left >= 3295 and avatar.left <= 3297 and not EntityExists(abs(int("plantEnemy")) + 3))
		{
			AddEntity(Point2f{ 3430, 154.f }, abs(int("plantEnemy")) + 3, Entity::EnemyType::plant, new Texture("plantEnemy.png"));
		}
		if (avatar.left >= 3530 and avatar.left <= 3532 and not EntityExists(abs(int("skeleton")) + 3))
		{
			AddEntity(Point2f{ 3732, 155.f }, abs(int("skeleton")) + 3, Entity::EnemyType::skeleton, new Texture("skeletonSpriteSheet.png"));
		}
		if (avatar.left >= 3680 and avatar.left <= 3682 and not EntityExists(abs(int("skeleton")) + 4))
		if (avatar.left >= 3780 and avatar.left <= 3950 and not EntityExists(abs(int("blob")) + 10) and not EntityExists(abs(int("MiniBoss"))))
		{
			AddEntity(Point2f{ 3480, 320.f }, abs(int("blob")) + 10, Entity::EnemyType::blob, new Texture("blob.png"), 120.f);
		}
		
	}
}

void EntityManager::Draw() const
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		m_pEntity[i]->Draw();
	}
	for (int i = 0; i < m_pHitMarkers.size(); i++)
	{
		m_pHitMarkers[i]->Draw();
	}
}

bool EntityManager::HitAvatar(const Rectf rect) const
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (utils::IsOverlapping(m_pEntity[i]->GetAttackZone(), Rectf{ rect.left, rect.bottom, rect.width * 2, rect.height * 2 }) and not m_pEntity[i]->GetIsFrozen()) 
		{
			if (m_pEntity[i]->GetEnemyType() == Entity::EnemyType::projectile) m_pEntity[i]->KillEntity();
			return true;
		}
	}
	return false;
}

void EntityManager::HitEntity(const Rectf& rect, const int damageDealt)
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (m_pEntity[i]->IsOverlapping(Rectf{ rect.left, rect.bottom, rect.width * 2, rect.height * 2 }) and not m_pEntity[i]->EntityIsHit())
		{
			std::cout << "Damage Dealt: " << damageDealt << std::endl;
			m_pEntity[i]->DealDamage(damageDealt);
			m_pHitMarkers.push_back(new HitMarker(Point2f{ rect.left, rect.bottom }));
		}
	}
}

void EntityManager::CastSpellOnEntity(const Powers::Power power, const Rectf& avatar, const int damageDealt, std::vector<FireBall*>& fireballs, const SoundEffect* freezeAudio, const SoundEffect* blastAudio, const SoundEffect* boltAudio) const
{
	switch (power)
	{
	case Powers::bolt:
		for (int i = 0; i < m_pEntity.size(); i++)
		{
			if (m_pEntity[i]->DistanceFromAvatar(avatar) <= 400)
			{
				m_pEntity[i]->DealDamage(damageDealt);
			}
			boltAudio->Play(0);
		}
		break;
	case Powers::blast:
		for (int i = 0; i < m_pEntity.size(); i++)
		{
			blastAudio->Play(0);
			if (m_pEntity[i]->IsHitBySpell(fireballs) and not m_pEntity[i]->EntityIsHit())
			{
				m_pEntity[i]->DealDamage(damageDealt);
			}
		}
		break;
	case Powers::bind:
		for (int i = 0; i < m_pEntity.size(); i++)
		{
			freezeAudio->Play(9);
			m_pEntity[i]->Freeze();
		}
		break;
	default:
		break;
	}
}

void EntityManager::CheckDeleteEntity(const SoundEffect* deathAudio)
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (m_pEntity[i]->GetHealth() <= 0)
		{
			m_pEntity[i]->KillEntity();
			switch (m_pEntity[i]->GetEnemyType())
			{
			case Entity::blob:
				break;
			case Entity::skeleton:
			case Entity::eyeball:
				deathAudio->Play(0);
				break;
			default:
				break;
			}
		}

		if (m_pEntity[i]->GetState() == Entity::ActionState::dead or m_pEntity[i]->GetPos().x < 0.f or m_pEntity[i]->GetPos().y < -200.f)
		{
			if (not (m_pEntity[i]->GetPos().x < 0.f or m_pEntity[i]->GetPos().y < -200.f)) //I don't know how to avoid this extra 'if'
			{
				switch (m_pEntity[i]->GetEnemyType())
				{
				case Entity::powerUp:
					m_Score += 500;
					break;
				case Entity::bomb:
				case Entity::projectile:
				case Entity::blob:
					m_Score += 100;
					break;
				case Entity::eyeball:
					m_Score += 400;
					break;
				case Entity::plant:
					m_Score += 600;
					break;
				case Entity::miniBoss:
					m_Score += 10000;
					m_pEntity[i]->KillEntity(); //create deactivation function
					break;
				case Entity::skeleton:
					m_Score += 1000;
					//deathAudio->Play(0);
					break;
				default:
					break;
				}
			}
			if (m_pEntity[i]->GetEnemyType() != Entity::miniBoss)
			{
				delete m_pEntity[i];
				m_pEntity[i] = m_pEntity[m_pEntity.size() - 1];
				m_pEntity.pop_back();
			}
		}
	}
}

int EntityManager::GetScore()
{
	const int tempScore{ m_Score };
	m_Score = 0;
	return tempScore;
}

bool EntityManager::GetStatueState() const
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (m_pEntity[i]->GetEnemyType() == Entity::EnemyType::powerUp and m_pEntity[i]->GetState() == Entity::ActionState::dying) return true;
	}
	return false;
}

Point2f EntityManager::GetStatuePos() const
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		if (m_pEntity[i]->GetEnemyType() == Entity::EnemyType::powerUp and m_pEntity[i]->GetState() == Entity::ActionState::dying)
		{
			return m_pEntity[i]->GetPos();
		}
	}
}

bool EntityManager::GetMiniBossIsDead() const
{
	for (int i = 0; i < m_pEntity.size(); i++)
	{
		return (m_pEntity[i]->GetEnemyType() == Entity::EnemyType::miniBoss and (m_pEntity[i]->GetState() == Entity::ActionState::deactivated or m_pEntity[i]->GetState() == Entity::ActionState::dead));
	}
}
