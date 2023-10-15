#include "pch.h"
#include "Entity.h"

Entity::Entity(const Point2f& bottomLeft, const float width, const float horSpeed, const float speed, const int id, const int health, const Texture* texture, const EnemyType enemyType) :
	m_AnimFrame{},
	m_Acceleration{ Vector2f{ 0, -981.0f } },
	m_HorSpeed{ horSpeed },
	m_JumpSpeed{ 800.f },
	m_IsGoingLeft{ true },
	m_IsHit{ false },
	m_AnimTime{},
	m_NrFramesPerSec{ 2 },
	m_NrOfFrames{ 2 },
	m_AttackZone{},
	m_Origin{ bottomLeft },
	m_pTexture{ texture },
	m_pBlastTexture{ new Texture("Blast.png") },
	m_Health{ health },
	m_IsFrozen{ false },
	m_Id{ id },
	m_EnemyType{ enemyType },
	m_FreezeTime{ }
{
	if (horSpeed == 0) m_Velocity = { Vector2f{ speed, speed } };
	else m_Velocity = { Vector2f{ horSpeed, speed } };
	m_Shape = { Rectf{ bottomLeft.x, bottomLeft.y, width, m_pTexture->GetHeight() } };
}


Entity::~Entity()
{
	delete m_pTexture;
	m_pTexture = nullptr;
	delete m_pBlastTexture;
	m_pBlastTexture = nullptr;
}

void Entity::Update(const float elapsedSec)
{
	if (m_IsFrozen) m_FreezeTime += elapsedSec;
	if (m_FreezeTime >= 3.f)
	{
		m_IsFrozen = false;
		m_FreezeTime = 0.f;
	}
}

bool Entity::IsOverlapping(const Rectf& rect) const
{
	const Rectf shape{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
	return (utils::IsOverlapping(rect, shape));
}

Point2f Entity::GetPos() const
{
	return Point2f{ m_Shape.left, m_Shape.bottom };
}

int Entity::GetId() const
{
	return m_Id;
}

float Entity::DistanceFromAvatar(const Rectf& avatar) const
{
	if (m_Shape.left >= avatar.left) return m_Shape.left - avatar.left + avatar.width * 2;
	else return avatar.left - m_Shape.left + m_Shape.width * 2;
}

Rectf Entity::GetAttackZone() const
{
	return m_AttackZone;
}

int Entity::GetHealth() const
{
	return m_Health;
}

bool Entity::EntityIsHit() const
{
	return m_IsHit;
}

void Entity::DealDamage(const int damageDealt)
{
	m_Health -= damageDealt;
	m_IsHit = true;
}

Entity::EnemyType Entity::GetEnemyType() const
{
	return m_EnemyType;
}

bool Entity::IsHitBySpell(std::vector<FireBall*>& fireballs) const
{
	for (int i = 0; i < fireballs.size(); i++)
	{
		if (utils::IsOverlapping(m_Shape, fireballs[i]->GetFireBall()))
		{
			delete fireballs[i];
			fireballs[i] = fireballs[fireballs.size() - 1];
			fireballs.pop_back();
			return true;
		}
	}
	return false;
}

void Entity::Freeze()
{
	m_IsFrozen = true;
}

bool Entity::GetIsFrozen() const
{
	return m_IsFrozen;
}

bool Entity::IsGoingLeft() const
{
	return m_IsGoingLeft;
}

int Entity::GetEntityId() const
{
	return m_Id;
}

