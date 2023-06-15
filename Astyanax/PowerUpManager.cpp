#include "pch.h"
#include "PowerUpManager.h"
#include <iostream>

PowerUpManager::PowerUpManager():
	m_PowerType{ PowerUp::PowerType::Strength }
{

}

PowerUpManager::~PowerUpManager()
{
	for (int i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
}

void PowerUpManager::AddItem(const Point2f& center, const int health, int magic, const int maxStrength)
{
	if (magic == 0) magic += 0.000001f;
	float probaHealth = 1 - (health / 20.f);
	float probaMagic = 1 - (magic / 20.f);
	if (probaHealth + probaMagic > 1)
	{
		const float total = probaHealth + probaMagic;
		probaHealth = (probaHealth * 100) / total;
		probaMagic = (probaMagic * 100) / total;
	}
	else
	{
		probaHealth *= 100;
		probaMagic *= 100;
	}
	const int probaUpgrade = 100 - probaHealth - probaMagic;
	const int random = rand() % 100 + 1;
	PowerUp::PowerType powerUp;
	if (random <= probaUpgrade)
	{
		const int random = rand() % 100 + 1;
		if (random <= 50) powerUp = PowerUp::PowerType::Strength;
		else powerUp = PowerUp::PowerType::Speed;
		if (maxStrength == 20) powerUp = PowerUp::PowerType::Speed;
	}
	else if (random > probaUpgrade and random <= probaUpgrade + probaHealth) powerUp = PowerUp::PowerType::Health;
	else powerUp = PowerUp::PowerType::Magic;
	m_pItems.push_back(new PowerUp(center, powerUp));
}

void PowerUpManager::Update(const float elapsedSec, Level*& level) const
{
	for (int i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Update(elapsedSec, level);
	}
}

void PowerUpManager::Draw() const
{
	for (int i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Draw();
	}
}

size_t PowerUpManager::Size() const
{
	return m_pItems.size();
}

bool PowerUpManager::HitItem(const Rectf& rect)
{
	for (int i = 0; i < m_pItems.size(); i++)
	{
		if (m_pItems[i]->IsOverlapping(Rectf{ rect.left, rect.bottom, rect.width * 2, rect.height * 2 }))
		{
			m_PowerType = m_pItems[i]->GetPowerType();
			delete m_pItems[i];
			m_pItems[i] = m_pItems[m_pItems.size() - 1];
			m_pItems.pop_back();
			return true;
		}
	}
	return false;
}

PowerUp::PowerType PowerUpManager::GetPowerType() const
{
	return m_PowerType;
}