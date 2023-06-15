#include "pch.h"
#include "Weapon.h"

Weapon::Weapon() :
	m_WeaponTexture{ "WeaponChoice.png" },
	m_TextInfo{ new Texture("       Damage:    **\n   Spell Damage:  **\nSpell Consumption:**", "zig.ttf", 18, Color4f{1.f,1.f,1.f,1.f}, true)},
	m_Width{ 59.f },
	m_Height{ 43.f },
	m_MaxHeight{ 474.f },
	m_MaxWidth{ 576.f },
	m_CurrentChoice{ 0 },
	m_IsSelected{ false }
{

}

Weapon::~Weapon()
{
	delete m_TextInfo;
}

void Weapon::Draw() const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	utils::FillRect(Rectf{ 0,0, m_MaxWidth, m_MaxHeight });

	const Rectf srcRect{ m_Width * m_CurrentChoice, 0, m_Width, m_Height };
	m_WeaponTexture.Draw(Rectf{ m_MaxWidth / 2 - m_Width, m_MaxHeight - m_Height * 4.5f, m_Width*4, m_Height*4 }, srcRect);

	m_TextInfo->Draw(Point2f{ 1 * m_MaxWidth / 3 - 50.f, m_MaxHeight - m_Height * 8.f});
}

void Weapon::Update(float elapsedSec)
{
	delete m_TextInfo;
	switch (m_CurrentChoice)
	{
	case 0:
		m_TextInfo = new Texture("       Damage:    **\n   Spell Damage:  **\nSpell Consumption:**", "zig.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f }, true);
			break;
	case 1:
		m_TextInfo = new Texture("       Damage:    *\n   Spell Damage:  ***\nSpell Consumption:*", "zig.ttf", 18, Color4f{1.f,1.f,1.f,1.f}, true);
			break;
	case 2:
		m_TextInfo = new Texture("       Damage:    ***\n   Spell Damage:  *\nSpell Consumption:***", "zig.ttf", 18, Color4f{1.f,1.f,1.f,1.f}, true);
			break;
	default:
		break;
	}
}

void Weapon::ChangeSelection(const int selection)
{
	m_CurrentChoice += selection;
}

int Weapon::GetSelection() const
{
	return m_CurrentChoice;
}