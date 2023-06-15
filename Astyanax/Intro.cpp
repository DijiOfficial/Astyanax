#include "pch.h"
#include "Intro.h"
#include <iostream>

Intro::Intro() :
	m_IntroSprite{Texture{"IntroImages.png"}},
	m_AstyanaxWalking{Texture{"AstyanaxWalking.png"}},
	m_TextSpeed{24},
	m_AnimSound{new SoundEffect{"Audio/02 Opening - Part 2 _ Ending - Part 3.mp3"}},
	m_TextClip{Rectf{0.f, 0.f, 256.f, 80.f}},
	m_Scrolling{false},
	m_ImgOffset{0.f},
	m_IsIntroOver{ false }
{
	m_AnimTime = 0;
	m_WalkingFrames = 0;
	m_Frame = 0;
	m_AnimFrame = 0;
	m_IntroTime = 0;
	m_EndLineWait = 0;
	m_CurrentLine = 0;
	m_CurrentChar = 0;
	m_WaitEndLine = false;
	RewriteNewLines();
	m_CurrentSentence = m_TextContent[m_CurrentLine];
	m_Text = new Texture(" ", "zig.ttf", 22, Color4f{1.f,1.f,1.f,1.f}, true);
	m_AnimSound->SetVolume(16);
}

Intro::~Intro()
{
	delete m_Text;
	delete m_AnimSound;
}

void Intro::Update(const float elapsedSec)
{
	if (not m_WaitEndLine)
	{
		m_IntroTime += elapsedSec;
		if (m_IntroTime >= 1.f / m_TextSpeed)
		{
			if (m_Frame == 1) ++m_AnimFrame %= 2;
			else m_AnimFrame = 0;
			++m_CurrentChar;
			m_IntroTime -= 1.f / m_TextSpeed;
			const std::string sentence = m_CurrentSentence.substr(0, m_CurrentChar);
			delete m_Text;
			m_Text = new Texture(sentence, "zig.ttf", 22, Color4f{ 1.f,1.f,1.f,1.f }, true);
		}
	}
	else
	{
		if (m_EndLineWait == 0.f and m_CurrentLine == 11) m_AnimSound->Play(0);
		m_EndLineWait += elapsedSec;
	}

	
	WaitEndLine();
	UpdateFrames();
	Scroll();
	UpdateFrame5(elapsedSec);
}

void Intro::Draw() const
{
	m_Text->Draw(Point2f{ 30.f, 80.f - m_Text->GetHeight() });

	int offset;
	if (m_Frame == 4) offset = 64.f;
	else if (m_Frame == 5) offset = 32 + abs(m_ImgOffset);
	else offset = 32.f;

	const Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width, m_TextClip.bottom + m_Frame * m_TextClip.height, m_TextClip.width, m_TextClip.height };
	const Rectf dstRect{ offset + m_ImgOffset, 256 , m_TextClip.width * 2, m_TextClip.height * 2 };
	m_IntroSprite.Draw(dstRect, srcRect);

	DrawFrame5And6();

}

void Intro::RewriteNewLines()
{
	while (std::getline(m_TextFile, m_CurrentSentence, '\n'))
	{
		for (std::string::iterator it = m_CurrentSentence.begin(); it != m_CurrentSentence.end(); ++it)
		{
			if (int(*it) == 92)
			{
				const std::size_t newline = m_CurrentSentence.find('\\');
				std::string part1 = m_CurrentSentence.substr(0, newline);
				std::string part2 = m_CurrentSentence.substr(newline + 2, m_CurrentSentence.size() - 1);
				m_CurrentSentence = part1 + "\n" + part2;
				break;
			}
		}
		m_TextContent.push_back(m_CurrentSentence);
	}
}

void Intro::UpdateFrames()
{
	switch (m_CurrentLine)
	{
	case 1:
		m_Frame = 0;
		break;
	case 2:
		m_Frame = 1;
		break;
	case 3:
		if (m_CurrentChar >= 2) m_AnimFrame = 1;
		else m_AnimFrame = 0;
		m_Frame = 2;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		if (not m_Scrolling) m_Frame = 3;
		break;
	case 8:
	case 9:
		m_Frame = 4;
		break;
	case 10:
	case 11:
		m_Frame = 5;
		break;
	case 12:
		m_ImgOffset = 0.f;
		m_Frame = 6;
		break;
	case 13:
	default:
		break;
	}
}

void Intro::WaitEndLine() 
{
	if (m_CurrentChar == m_CurrentSentence.size())
	{
		m_WaitEndLine = true;
		if (m_EndLineWait >= 0.5f)
		{
			if (m_CurrentLine == 7 and m_EndLineWait <= 2.5f)//?
			{
				m_Scrolling = true;
			}
			if (not m_Scrolling)
			{
				++m_CurrentLine;
				m_CurrentChar = 0;
				m_EndLineWait = 0.f;
				m_WaitEndLine = false;
				if (m_CurrentLine<= 12) m_CurrentSentence = m_TextContent[m_CurrentLine];
				else 
				{
					delete m_AnimSound;
					m_AnimSound = { new SoundEffect{"Audio/glitchy8-85509.mp3"} };
					m_AnimSound->Play(0);
					m_IsIntroOver = true;
				}
			}
		}
	}
}

void Intro::Scroll()
{
	if (m_Scrolling)
	{
		m_ImgOffset -= 1.f;
		if (m_ImgOffset == 0)
		{
			m_Scrolling = false;
		}
		if (m_ImgOffset <= -428) //viewport / 2 + 32 offset original + (frame width / 2) * 2
		{
			m_Frame += 1;
			m_ImgOffset = 129.f; // frame width * 2 - 32 offset original + 1
		}
	}
}

void Intro::UpdateFrame5(const float elapsedSec)
{
	if (m_Frame == 5)
	{
		m_ImgOffset -= 0.25f;
		constexpr int frames{ 4 };
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= 0.5f / frames)
		{
			++m_WalkingFrames %= frames;
			m_AnimTime -= 0.5f / frames;
		}

		if (m_WaitEndLine and m_CurrentLine == 11)
		{
			m_WalkingFrames = 4;
			m_ImgOffset += 0.25f;
		}
	}
}

void Intro::DrawFrame5And6() const
{
	if (m_Frame == 5)
	{
		constexpr float width{ 30.f };
		constexpr float height{ 37.f };
		const Rectf srcRect{ m_WalkingFrames * width, height, width, height };
		const Rectf dstRect{ 576.f + width * 2 + m_ImgOffset, 317 , width * 2, height * 2 }; //317 viewport heihgt / 2 + background height/2 *2
		m_AstyanaxWalking.Draw(dstRect, srcRect);

		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
		utils::FillRect(Rectf{ 256 * 2 + 32, 256, 32 , 256 });
	}
}

bool Intro::GetIntroState() const
{
	return m_IsIntroOver;
}

void Intro::SkipIntro()
{
	m_IsIntroOver = true;
}