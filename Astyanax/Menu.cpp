#include "pch.h"
#include "Menu.h"
#include <iostream>
Menu::Menu(const Rectf size) :
	m_BackgroundTexture{ "MenuOriginal.png" },
	m_TextTitle{Texture("Burgisser Dylan\n     1GD10\n   ASTYANAX", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_TextStart{Texture("Press Enter to start", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_TextHighScores{Texture("HighScores", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_Viewport{size},
	m_AnimeTime{ 0.f },
	m_DisplayText{ true },
	m_IsMenuActive{ false },
	m_MenuOptionsCount{ 0 }
{

}

void Menu::Draw() const
{
	Point2f startPos{ m_Viewport.width * 0.5f - m_TextStart.GetWidth() * 0.5f, m_Viewport.height * 0.5f - m_TextStart.GetHeight() * 0.5f };
	const Texture scoreboard{ Texture("Scoreboard", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true) };
	m_BackgroundTexture.Draw(Rectf{ 0.f, 80.f, m_Viewport.width, m_Viewport.height });
	m_TextTitle.Draw(Point2f{ m_Viewport.width / 2 - m_TextTitle.GetWidth()/4, m_Viewport.height / 2 - m_TextTitle.GetHeight() * 3});
	
	if (m_DisplayText or m_MenuOptionsCount == 1) 
		m_TextStart.Draw(startPos);

	if (m_DisplayText or m_MenuOptionsCount == 0)
		scoreboard.Draw(Point2f{ m_Viewport.width * 0.5f - scoreboard.GetWidth() * 0.5f, startPos.y - 35 });
	
	startPos.x -= 20;
	startPos.y += 7;

	utils::FillTriangle(Point2f{ startPos.x - 6,  -m_MenuOptionsCount * (35) + startPos.y + 15 },
						Point2f{ startPos.x - 6,  -m_MenuOptionsCount * (35) + startPos.y - 5 },
						Point2f{ startPos.x + 10, -m_MenuOptionsCount * (35) + startPos.y + 5 });
}

void Menu::Update(const float elapsedSec)
{
	m_AnimeTime += elapsedSec;
	if (m_AnimeTime >= 0.2f)
	{
		m_AnimeTime = 0;
		m_DisplayText = !m_DisplayText;
	}
}