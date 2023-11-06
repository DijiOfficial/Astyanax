#pragma once
#include <map>
#include <string>

class Scoreboard final
{
public:
	Scoreboard(const Point2f& viewport);

	void Draw() const;
	void AddScore(const std::string name, const int score);
	void ReadFile();
	void PrintScore();

private:
	std::string m_UserName;
	std::multimap<int, std::string, std::greater<int>> m_Scoreboard;
	const Point2f m_Viewport;
};

