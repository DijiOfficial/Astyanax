#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(const Point2f& bottomLeft, const int horSpeed, const int speed, const Texture* texture, const Entity::EnemyType enemyType) : Entity(bottomLeft, 8.f, horSpeed, speed, abs(int("Projectile")), 1, texture, enemyType),
	m_ActionState{ ActionState::alive }
{
}

void Projectile::Draw() const
{
	m_pTexture->Draw(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 });

	if (m_Debugger.m_ShowHitBox)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::DrawRect(Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2 , m_Shape.height * 2 });
	}
}

void Projectile::Update(const float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio)
{
	Entity::Update(elapsedSec);

	if (not m_IsFrozen)
	{
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		if (m_Velocity.y >= m_Acceleration.y and m_Velocity.y != 0) m_Velocity.y += m_Acceleration.y * elapsedSec;
	}

	if (level->IsCollidingVertically(m_Shape) or level->IsCollidingWithWorld(m_Shape)) KillEntity();
	m_AttackZone = Rectf{ m_Shape.left, m_Shape.bottom, m_Shape.width * 2, m_Shape.height * 2 };
}

void Projectile::KillEntity()
{
	m_ActionState = ActionState::dead;
}

Entity::ActionState Projectile::GetState() const
{
	return m_ActionState;
}

void Projectile::UpdateFrames(float elapsedSec)
{

}