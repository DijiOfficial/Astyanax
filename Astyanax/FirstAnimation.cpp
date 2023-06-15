#include "pch.h"
#include "FirstAnimation.h"


FirstAnimation::FirstAnimation() :
	m_FirstAnimationSprite{Texture{"AnimationSpriteSheet.png"}},
	m_CutieSprite{Texture{"Cutie.png"}},
	m_TextSpeed{24},
	m_AnimSound{new SoundEffect("Audio/BoltAttackShort.mp3")},
	m_TextClip{Rectf{0.f, 0.f, 145.f, 105.f}},
	m_Scrolling{true},// frame width * 2 + 1
	m_ImgOffset{290.f + 1.f},
	m_IsFirstAnimationOver{ true },
	m_DrawWhiteBackGround{ false }
{
	m_NrOfFrames = 0;
	m_AnimTime = 0;
	m_CutieFrames = 0;
	m_Frame = 1;
	m_AnimFrame = 0;
	m_FirstAnimationTime = 0;
	m_EndLineWait = 0;
	m_CurrentLine = 0;
	m_CurrentChar = 0;
	m_WaitEndLine = true;
	RewriteNewLines();
	m_CurrentSentence = m_TextContent[m_CurrentLine];
	m_Text = new Texture(" ", "zig.ttf", 22, Color4f{ 1.f,1.f,1.f,1.f }, true);
	m_AnimSound->SetVolume(8);
}

FirstAnimation::~FirstAnimation()
{
	delete m_Text;
	delete m_AnimSound;
}

void FirstAnimation::Update(const float elapsedSec)
{
	++m_NrOfFrames %= 4;

	if (not m_WaitEndLine)
	{
		m_FirstAnimationTime += elapsedSec;
		if (m_FirstAnimationTime >= 1.f / m_TextSpeed)
		{
			++m_AnimFrame %= 3;
			++m_CurrentChar;
			m_FirstAnimationTime -= 1.f / m_TextSpeed;
			const std::string sentence = m_CurrentSentence.substr(0, m_CurrentChar);
			delete m_Text;
			m_Text = new Texture(sentence, "zig.ttf", 22, Color4f{ 1.f,1.f,1.f,1.f }, true);
		}
	}
	else
	{
		m_EndLineWait += elapsedSec;
	}

	if (m_Frame == 3)
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime >= 0.5f / 2)
		{
			++m_CutieFrames %= 2;
			m_AnimTime = 0;
		}
	}

	WaitEndLine();
	UpdateFrames();
	Scroll();
}

void FirstAnimation::Draw() const
{
	m_Text->Draw(Point2f{ 30.f, 80.f - m_Text->GetHeight() });

	int offset{ 143 };
	if (m_Frame == 1) offset = 576 / 2;
	else if (m_Frame == 3) offset = 250.f;

	const Rectf srcRect{ m_TextClip.left + m_AnimFrame * m_TextClip.width, m_TextClip.bottom + m_Frame * m_TextClip.height, m_TextClip.width, m_TextClip.height };
	const Rectf dstRect{ offset + m_ImgOffset, 206 , m_TextClip.width * 2, m_TextClip.height * 2 };
	if (m_CurrentLine != 18) m_FirstAnimationSprite.Draw(dstRect, srcRect);
	if (m_Frame == 3 and m_CurrentLine != 18) DrawCutie();
	if (m_DrawWhiteBackGround)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::FillRect(Rectf{ 0.f,0.f, 576.f, 474.f });
	}

}

void FirstAnimation::RewriteNewLines()
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

void FirstAnimation::WaitEndLine()
{
	if (m_CurrentChar == m_CurrentSentence.size())
	{
		m_WaitEndLine = true;
		if (m_EndLineWait >= 0.5f)
		{
			if (not m_Scrolling)
			{
				++m_CurrentLine;
				m_CurrentChar = 0;
				m_EndLineWait = 0.f;
				m_WaitEndLine = false;
				if (m_CurrentLine <= 25) m_CurrentSentence = m_TextContent[m_CurrentLine];
				else m_IsFirstAnimationOver = true;
			}
		}
	}
}

void FirstAnimation::UpdateFrames()
{
	switch (m_CurrentLine)
	{
	case 0:
		m_Frame = 1;
		m_AnimFrame = 0;
		break;
	case 1:
	case 2:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_Frame = 2;
		break;
	case 3:
	case 4:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		m_Frame = 3;
		m_AnimFrame = 0;
		break;
	case 18:
		m_AnimSound->Play(0);
		if (m_NrOfFrames == 0) m_DrawWhiteBackGround = !m_DrawWhiteBackGround;
		break;
	case 19:
	case 20:
	case 21:
	case 22:
		m_DrawWhiteBackGround = false;
		m_Frame = 4;
		m_AnimFrame = 0;
		break;
	case 23:
		m_AnimFrame = 0;
		m_Frame = 5;
		break;
	case 24:
		m_Frame = 6;
		break;
	case 25:
		m_AnimFrame %= 2;
		m_Frame = 0;
		break;
	default:
		break;
	}
}

void FirstAnimation::Scroll()
{
	if (m_Scrolling)
	{
		m_ImgOffset -= 1.f;
		if (m_ImgOffset == 0)
		{
			m_Scrolling = false;
			m_WaitEndLine= false;
			m_EndLineWait = 0.f;
		}
	}
}

void FirstAnimation::DrawCutie() const
{
	constexpr float width{ 46 };
	constexpr float height{ 54 };
	constexpr float offset{ 85.f };
	const Rectf srcRect{ m_CutieFrames * width, 0, width, height };
	const Rectf dstRect{ offset, 350.f , width * 2, height * 2 };
	m_CutieSprite.Draw(dstRect, srcRect);
}

bool FirstAnimation::GetFirstAnimationState() const
{
	return m_IsFirstAnimationOver;
}

void FirstAnimation::SkipFirstAnimation()
{
	m_IsFirstAnimationOver = true;
}
