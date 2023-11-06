#pragma once
#include "Texture.h"
#include "utils.h"
#include "Avatar.h"

class HUD
{
public:
	explicit HUD(Rectf background);
	~HUD();
	HUD(const HUD& other) = delete;
	HUD(HUD&& other) noexcept = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD& operator=(HUD&& other) noexcept = delete;
	void Draw() const;
	void Update(const int score, const int lives, const Avatar* avatar);

private:
	const Rectf m_Background;
	const Texture m_ScoreText;
	const Texture m_LifeTexture;
	Texture* m_ScoreValue;
	Texture* m_Lives;
	Texture* m_Power;
	Texture m_Level;
	Texture m_PowerLifeSpellText;
	Texture m_PowerLifeSpellTextRedBackground;
	Texture m_Life;
	Texture m_Spell;
	Rectf m_SrcRect;
	int m_Health;
	int m_SpellGauge;
	int m_Strength;
};


