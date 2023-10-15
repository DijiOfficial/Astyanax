#include "pch.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(const Rectf size) :
	m_TextMenu{ Texture(" Pause Menu ", "zig.ttf", 28, Color4f{1.f, 1.f, 1.f, 1.f}, true) },
	m_MenuOptions{ Texture("CONTINUE\n\nMUSIC OFF\n\nMUSIC ON\n\nTOGGLE VFX\n\nSCOREBOARD\n\nRESET", "zig.ttf", 24, Color4f{1.f, 1.f, 1.f, 1.f}, true) },
	m_Viewport{ size },
	m_MenuPadding{ 45.f },
	m_MenuOptionsCount{ 2 }
{
	m_MenuSize = Rectf{ size.width / 4, 100, size.width / 2, 2 * size.height / 3 };
}

void PauseMenu::Draw() const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,0.75f });
	utils::FillRect(m_Viewport);
	
	utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	utils::DrawRect(m_MenuSize, 4.f);

	m_TextMenu.Draw(Point2f{ m_Viewport.width / 2 - m_TextMenu.GetWidth() / 2 , m_MenuSize.bottom + m_MenuSize.height + m_TextMenu.GetHeight() / 2 });
	m_MenuOptions.Draw(Point2f{ m_MenuSize.left + m_MenuPadding, m_MenuSize.bottom + m_MenuPadding / 2 });

	utils::FillTriangle(Point2f{ m_MenuSize.left + m_MenuPadding / 2 - 6,  -m_MenuOptionsCount * (m_MenuOptions.GetHeight() / 6 + 4) + m_MenuSize.bottom + m_MenuSize.height - m_MenuPadding + 15 },
						Point2f{ m_MenuSize.left + m_MenuPadding / 2 - 6,  -m_MenuOptionsCount * (m_MenuOptions.GetHeight() / 6 + 4) + m_MenuSize.bottom + m_MenuSize.height - m_MenuPadding - 5 },
						Point2f{ m_MenuSize.left + m_MenuPadding / 2 + 10, -m_MenuOptionsCount * (m_MenuOptions.GetHeight() / 6 + 4) + m_MenuSize.bottom + m_MenuSize.height - m_MenuPadding + 5 });
}

