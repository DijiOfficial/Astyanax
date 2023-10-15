#pragma once
#include <fstream>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "SoundEffect.h"

class FirstAnimation
{
public:
	FirstAnimation();
	~FirstAnimation();
	FirstAnimation(const FirstAnimation& other) = delete;
	FirstAnimation(FirstAnimation&& other) noexcept = delete;
	FirstAnimation& operator=(const FirstAnimation& other) = delete;
	FirstAnimation& operator=(FirstAnimation&& other) noexcept = delete;
	void Update(float elapsedSec);
	void Draw() const;
	bool GetFirstAnimationState() const;
	void SkipFirstAnimation();
	void SetActive() { m_IsFirstAnimationOver = false; };

private:
	//Animation variables
	Texture* m_Text;
	Texture m_FirstAnimationSprite;
	Texture m_CutieSprite;
	const float m_TextSpeed;
	float m_FirstAnimationTime;
	float m_EndLineWait;
	std::string m_CurrentSentence;
	std::ifstream m_TextFile{ "FirstAnimation.txt" };
	std::vector<std::string> m_TextContent;
	int m_CurrentLine;
	int m_CurrentChar;
	SoundEffect* m_AnimSound;

	float m_AnimTime;
	int m_CutieFrames;
	bool m_WaitEndLine;
	const Rectf m_TextClip;
	int m_AnimFrame;
	int m_Frame;
	bool m_Scrolling;
	float m_ImgOffset;
	bool m_IsFirstAnimationOver;
	bool m_DrawWhiteBackGround;
	int m_NrOfFrames;

	void RewriteNewLines();
	void UpdateFrames();
	void WaitEndLine();
	void Scroll();
	void DrawCutie() const;
};

