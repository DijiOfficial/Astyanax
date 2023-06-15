#pragma once
class Debug
{
public:
	static bool m_ShowHitBox;
	static bool m_InfinitPower;
	static bool m_OnePunch;

	Debug();
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;
};

