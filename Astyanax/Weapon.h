#pragma once
#include "Texture.h"
#include "utils.h"

class Weapon
{
public:
	Weapon();
	~Weapon();
	Weapon(const Weapon& other) = delete;
	Weapon(Weapon&& other) noexcept = delete;
	Weapon& operator=(const Weapon& other) = delete;
	Weapon& operator=(Weapon&& other) noexcept = delete;
	void Draw() const;
	void Update(float elapsedSec);
	void ChangeSelection(const int selection);
	int GetSelection() const;
	void SetUnselected() { m_IsSelected = false; };
	void SetSelected() { m_IsSelected = true; };
	bool GetIsSelected() const { return m_IsSelected; };
private:
	Texture m_WeaponTexture;
	Texture* m_TextInfo;
	Texture* m_MenuInfo;
	Texture* m_MenuInfo2;
	const float m_Width;
	const float m_Height;
	const float m_MaxHeight;
	const float m_MaxWidth;
	int m_CurrentChoice;
	bool m_IsSelected;
};

