#pragma once
#include "Texture.h"
#include "utils.h"
#include "FireBall.h"

class Powers
{
public:
	const enum Power
	{
		bolt,
		blast,
		bind
	};

	explicit Powers(Power power);
	~Powers();
	Powers(const Powers& other) = delete;
	Powers(Powers&& other) noexcept = delete;
	Powers& operator=(const Powers& other) = delete;
	Powers& operator=(Powers&& other) noexcept = delete;
	void Draw() const;
	void Update(float elapsedSec);
	void ChangePower();
	Power GetCurrentPower() const;
	void AddFireballs(Rectf avatar);
	std::vector<FireBall*>& GetFireBallVector();

private:
	Powers::Power m_CurrentPower;
	std::vector<FireBall*> m_pFireballs;
};

