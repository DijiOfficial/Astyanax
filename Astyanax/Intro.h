#pragma once
#include <fstream>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "SoundEffect.h"

class Intro
{
public:
	Intro();
	~Intro();
	Intro(const Intro& other) = delete;
	Intro(Intro&& other) noexcept = delete;
	Intro& operator=(const Intro& other) = delete;
	Intro& operator=(Intro&& other) noexcept = delete;
	void Update(float elapsedSec);
	void Draw() const;
	bool GetIntroState() const;
	void SkipIntro();
private:
	//Animation variables
	Texture* m_Text;
	Texture m_IntroSprite;
	Texture m_AstyanaxWalking;
	const float m_TextSpeed;
	float m_IntroTime;
	float m_EndLineWait;
	std::string m_CurrentSentence;
	std::ifstream m_TextFile{ "IntroText.txt" };
	std::vector<std::string> m_TextContent;
	int m_CurrentLine;
	int m_CurrentChar;
	SoundEffect* m_AnimSound;

	float m_AnimTime;
	int m_WalkingFrames;
	bool m_WaitEndLine;
	const Rectf m_TextClip;
	int m_AnimFrame;
	int m_Frame;
	bool m_Scrolling;
	float m_ImgOffset;
	bool m_IsIntroOver;

	void RewriteNewLines();
	void UpdateFrames();
	void WaitEndLine();
	void Scroll();
	void UpdateFrame5(float elapsedSec);
	void DrawFrame5And6() const;
};

