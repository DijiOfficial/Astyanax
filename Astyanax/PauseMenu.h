#pragma once
#include "Texture.h"
#include "utils.H"
class PauseMenu
{
public:
	PauseMenu(Rectf size);
	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;
	void Draw() const;
	void IncrementMenuCount() { m_MenuOptionsCount = ++m_MenuOptionsCount % 6; };
	void DecrementMenuCount() { m_MenuOptionsCount = (m_MenuOptionsCount - 1) < 0 ? 0 : --m_MenuOptionsCount; };
	void ResetCount() { m_MenuOptionsCount = 0; };
	int GetMenuCount() const { return m_MenuOptionsCount; };

private:
	Texture m_TextMenu;
	Texture m_MenuOptions;
	const float m_MenuPadding;
	int m_MenuOptionsCount;

	Rectf m_Viewport;
	Rectf m_MenuSize;
};

