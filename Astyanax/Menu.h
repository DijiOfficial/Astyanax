#pragma once
#include "Texture.h"
#include "utils.h"

class Menu
{
public:
	explicit Menu(Rectf size);
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) noexcept = delete;
	Menu& operator=(const Menu& other) = delete;
	Menu& operator=(Menu&& other) noexcept = delete;
	void Draw() const;
	void Update(float elapsedSec);
	bool GetMenuActive() const { return m_IsMenuActive; };
	void StartGame() { m_IsMenuActive = false; };
	void SetActive() { m_IsMenuActive = true; };

private:
	Texture m_BackgroundTexture;
	Texture m_TextTitle;
	Texture m_TextStart;
	Texture m_TextHighScores;
	Rectf m_Viewport;
	float m_AnimeTime;
	bool m_DisplayText;
	bool m_IsMenuActive;
};

