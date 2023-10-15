#pragma once
#include "PowerUp.h"
#include <vector>
#include "structs.h"

class PowerUpManager final
{
public:
	PowerUpManager( );
	~PowerUpManager( );
	PowerUpManager(const PowerUpManager&) = delete;
	PowerUpManager& operator=(const PowerUpManager&) = delete;
	PowerUpManager(PowerUpManager&&) = delete;
	PowerUpManager& operator=(PowerUpManager&&) = delete;

	void AddItem( const Point2f &center, const int health, int magic, const int maxStrength);
	void Update( float elapsedSec, Level*& level) const;
	void Draw( ) const;
	PowerUp::PowerType GetPowerType() const;
	size_t Size( ) const;
	bool HitItem( const Rectf& rect ) ;

private:
	std::vector<PowerUp*> m_pItems;
	PowerUp::PowerType m_PowerType;
};
