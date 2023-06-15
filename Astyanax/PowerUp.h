#pragma once
#include "Texture.h"
#include "utils.h"
#include "Level.h"

class PowerUp final
{
public:

	const enum PowerType
	{
		Strength = 0,
		Speed,
		Health,
		Magic,
	};

	explicit PowerUp(const Point2f& bottomLeft, PowerType power );
	~PowerUp();
	PowerUp(const PowerUp& other) = delete;
	PowerUp(PowerUp&& other) noexcept = delete;
	PowerUp& operator=(const PowerUp& other) = delete;
	PowerUp& operator=(PowerUp&& other) noexcept = delete;
	void Update( float elapsedSec, Level*& level);
	void Draw( ) const;
	bool IsOverlapping(const Rectf& rect ) const;
	void MovePowerUp(float elapsedSec);
	PowerType GetPowerType() const;

private:
	const Texture* m_pTexture;
	Rectf m_TextClip;
	Rectf m_Shape;
	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	const PowerType m_PowerType;
};


