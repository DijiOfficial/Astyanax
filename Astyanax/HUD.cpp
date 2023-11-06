#include "pch.h"
#include "HUD.h"

HUD::HUD(const Rectf background) :
	m_Background(background),
	m_ScoreText{Texture("SCORE", "zig.ttf", 18, Color4f{1.f, 1.f, 1.f, 1.f})},
	m_LifeTexture{Texture("lives.png")},
	m_ScoreValue{new Texture("0000000", "zig.ttf", 18, Color4f{1.f, 1.f, 1.f, 1.f})},
	m_Lives{new Texture("=02", "zig.ttf", 18, Color4f{1.f, 1.f, 1.f, 1.f})},
	m_Power{new Texture("Powers.png")},
	m_Level{Texture("R1-1", "zig.ttf", 18, Color4f{1.f, 1.f, 1.f, 1.f})},
	m_PowerLifeSpellText{
		Texture("PW\n\nLF\nSP", "zig.ttf", 18, Color4f{247.f / 255.f, 190.f / 255.f, 57.f / 255.f, 1.f}, true)
	},
	m_PowerLifeSpellTextRedBackground{
		Texture("PW\n\nLF\nSP", "zig.ttf", 18, Color4f{222 / 255.f, 40 / 255.f, 0.f, 1.f}, true)
	},
	m_Life{Texture("heart.png")},
	m_Spell{Texture("spell.png")},
	m_SrcRect{0.f, 0.f, 16.f, 16.f},
	m_Health{20},
	m_SpellGauge{ 0 },
	m_Strength{ 0 }
{

}

HUD::~HUD()
{
	delete m_ScoreValue;
	m_ScoreValue = nullptr;
	delete m_Lives;
	m_Lives = nullptr;
	delete m_Power;
	m_Power = nullptr;
}

void HUD::Update(const int score, const int lives, const Avatar* avatar)
{
	delete m_ScoreValue;
	delete m_Lives;
	std::string zeroes{};
	if (score == 0) zeroes = "000000";
	else
	{
		for (int i = 0; i < 6 - log10(score); i++)
		{
			zeroes += "0";
		}
	}
	m_ScoreValue = { new Texture(zeroes + std::to_string(score), "zig.ttf", 18, Color4f{1.f,1.f,1.f,1.f} )};
	
	if (lives < 10) zeroes = "0";
	else zeroes = "";
	m_Lives = { new Texture("=" + zeroes + std::to_string(lives), "zig.ttf", 18, Color4f{1.f,1.f,1.f,1.f} )};
	m_SrcRect = Rectf{ avatar->GetCurrentPower() * 16.f, 0.f, 16.f, 16.f };
	m_Health = avatar->GetHealth();
	m_SpellGauge = avatar->GetMana();
	m_Strength = avatar->GetStrength();
}

void HUD::Draw() const
{

	//m_pLeftTexture->Draw(m_BottomLeft);
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(m_Background);

	m_ScoreText.Draw(Point2f{ 30.f, 80.f - m_ScoreText.GetHeight() });
	m_ScoreValue->Draw(Point2f{ 15.f, 80.f - 2 * m_ScoreValue->GetHeight() });
	m_LifeTexture.Draw(Point2f{ 15.f, 80.f - 3 * m_ScoreText.GetHeight() + 2 });
	m_Lives->Draw(Point2f{ 30.f, 80.f - 3 * m_Lives->GetHeight() });
	m_Level.Draw(Point2f{ 15.f, 80.f - 4 * m_Level.GetHeight() });
	m_PowerLifeSpellTextRedBackground.Draw(Point2f{ 22.f + m_ScoreValue->GetWidth(), 78.f - 4 * m_ScoreValue->GetHeight() });
	m_PowerLifeSpellText.Draw(Point2f{ 20.f + m_ScoreValue->GetWidth(), 80.f - 4 * m_ScoreValue->GetHeight() });
	
	for (int i = 0; i < m_Health; i++)
	{
		m_Life.Draw(Rectf{ 60.f + m_ScoreValue->GetWidth() + i * 20, 80.f - 3 * m_Level.GetHeight(), 18, 16});
	}
	for (int i = 0; i < m_SpellGauge; i++)
	{
		m_Spell.Draw(Rectf{ 60.f + m_ScoreValue->GetWidth() + i * 20, 80.f - 4 * m_Level.GetHeight(), 18, 16 });
	}
	for (int i = 0; i < m_Strength; i++)
	{
		utils::SetColor(Color4f{ 222 / 255.f, 40 / 255.f, 0.f, 1.f });
		utils::FillRect(Rectf{ 60.f + m_ScoreValue->GetWidth() + i * 20 - 2, 80.f - m_Level.GetHeight() - 2, 22.f, 16.f });
		
	}
	for (int i = 0; i < m_Strength; i++)
	{
		utils::SetColor(Color4f{ 247.f / 255.f, 190.f / 255.f, 57.f / 255.f,1.f });
		utils::FillRect(Rectf{ 60.f + m_ScoreValue->GetWidth() + i * 20, 82.f - m_Level.GetHeight(), 20.f, 14.f });

	}


	const Rectf dstRect{ 75.f, 80.f - 4 * m_Level.GetHeight() + 2 , m_SrcRect.width * 2, m_SrcRect.height * 2 };
	m_Power->Draw(dstRect, m_SrcRect);
}

