#include "pch.h"
#include "Menu.h"

Menu::Menu(const Rectf size) :
	m_BackgroundTexture{ "Menu.png" },
	m_TextTitle{Texture("Burgisser Dylan\n     1GD10\n   ASTYANAX", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_TextStart{Texture("Press Enter to start", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_TextHighScores{Texture("HighScores", "zig.ttf", 22, Color4f{1.f, 1.f, 1.f, 1.f}, true)},
	m_Viewport{size},
	m_AnimeTime{ 0.f },
	m_DisplayText{ true },
	m_IsMenuActive{ false }
{

}

void Menu::Draw() const
{
	const Point2f startPos{ m_Viewport.width / 2 - m_TextStart.GetWidth() / 2, m_Viewport.height / 2 - m_TextStart.GetHeight() * 2 };

	m_BackgroundTexture.Draw(Rectf{ 0.f, 80.f, m_Viewport.width, m_Viewport.height });
	m_TextTitle.Draw(Point2f{ m_Viewport.width / 2 - m_TextTitle.GetWidth()/4, m_Viewport.height / 2 - m_TextTitle.GetHeight() * 3});
	
	if (m_DisplayText) m_TextStart.Draw(startPos);
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