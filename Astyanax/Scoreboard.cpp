#include "pch.h"
#include "Scoreboard.h"
#include <fstream>
#include <iostream>
#include "Texture.h"
#include "utils.h"

Scoreboard::Scoreboard(const Point2f& viewport)
    : m_Viewport{ viewport }
{
    ReadFile();
}

void Scoreboard::Draw() const
{
    const Texture highScore{ Texture("HIGH SCOREs", "zig.ttf", 24, Color4f{1.f, 1.f, 1.f, 1.f}) };
    const Texture line2{ Texture("RANK   NAME       SCORE", "zig.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}) };
    const Texture ranks{ Texture("1ST\n\n2ND\n\n3RD\n\n4TH\n\n5TH\n\n6TH\n\n7TH\n\n8TH\n\n9TH\n\n10TH", "zig.ttf", 18, Color4f{ 247.f / 255.f, 190.f / 255.f, 57.f / 255.f, 1.f }, true) };

    utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
    utils::FillRect(Rectf{ 0, 0, m_Viewport.x, m_Viewport.y });

    highScore.Draw(Point2f{ m_Viewport.x * 0.5f - highScore.GetWidth() * 0.5f, m_Viewport.y - highScore.GetHeight() * 1.5f });
    ranks.Draw(Point2f{ 2 * m_Viewport.x / 5.f - ranks.GetWidth() * 0.2f, highScore.GetHeight() });
    
    std::string namesString{};
    std::string valuesString{};
    int it{ 0 };
    for (const std::pair<int, std::string>& pair : m_Scoreboard)
    {
        std::string zeroes{};
        if (pair.first == 0) 
            zeroes = "000000";
        else
            for (int i = 0; i < 6 - log10(pair.first); i++)
                zeroes += "0";

        if (++it == 10)
        {
            namesString += pair.second; 
            valuesString += zeroes + std::to_string(pair.first);
            break;
        }
        valuesString += zeroes + std::to_string(pair.first) + "\n\n";
        namesString += pair.second + "\n\n";
    }
    //std::cout << namesString << std::endl;
    const Texture names{ Texture(namesString, "zig.ttf", 18, Color4f{ 247.f / 255.f, 190.f / 255.f, 57.f / 255.f, 1.f }, true) };
    names.Draw(Point2f{ 3 * m_Viewport.x / 5.f - names.GetWidth() * 0.2f, highScore.GetHeight() });
    
    const Texture values{ Texture(valuesString, "zig.ttf", 18, Color4f{ 247.f / 255.f, 190.f / 255.f, 57.f / 255.f, 1.f }, true) };
    values.Draw(Point2f{4 * m_Viewport.x / 5.f- values.GetWidth() * 0.2f, highScore.GetHeight() });
}

void Scoreboard::AddScore(const std::string name, const int score)
{
    const std::string fileName{ "scoreboard.txt" };
    std::ofstream file{ fileName, std::ios_base::app | std::ios_base::out };
    std::string text{ name + ":" + std::to_string(score) + "\n"};
    file << text;
    file.flush();
    file.close();

    m_Scoreboard.insert(std::pair<int, std::string>(score, name));
}

void Scoreboard::PrintScore()
{
    for (auto item : m_Scoreboard)
    {
        std::cout << item.first << " " << item.second << std::endl;
    }
}

void Scoreboard::ReadFile()
{
    std::ifstream file{ "scoreboard.txt" };

    std::string line, text;
    while (std::getline(file, line, '\n'))
    {
        std::string part1 = line.substr(0, line.find(":"));
        std::string part2 = line.substr(line.find(":") + 1, line.size());

        m_Scoreboard.insert(std::pair<int, std::string>(std::stoi(part2), part1));
    }
}


