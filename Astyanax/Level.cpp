#include "pch.h"
#include "Level.h"
#include "SvgParser.h"
#include <iostream>

Level::Level() :
	m_pBackgroundTexture{ new Texture{"test3.png" } },
	m_pBackgroundFullbright{ new Texture{"1-1RemliaFullbright.png" } },
	m_Boundaries{ 0.f, 90.f, m_pBackgroundTexture->GetWidth() * 2 - 796.f, m_pBackgroundTexture->GetHeight() * 2  },
	m_ClipHeight{ 192.f },
	m_ClipWidth{ 192.f * 4 },
	m_IsPlayerLockedInBounds{ false },
	m_IsPlayerInBossRoom{ false }
{
	SVGParser::GetVerticesFromSvgFile("level1test.svg", m_Vertices);
	for (int j = 0; j < m_Vertices.size(); j++) 
	{
		for (int i = 0; i < m_Vertices[j].size(); i++)
		{
			m_Vertices[j][i].x *= 2;
			m_Vertices[j][i].y *= 2;
			m_Vertices[j][i].y += 90;
		}
	}
}

Level::~Level()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;
	delete m_pBackgroundFullbright;
	m_pBackgroundFullbright = nullptr;
}

void Level::DrawBackground(const Rectf viewport, const Rectf& avatar, const int frame) const
{
	const float width{ viewport.width * (8 / 3.f) };
	int Sector{};
	if (avatar.left < width) Sector = 0;
	else Sector = { int((avatar.left - (width / 2)) / int(width)) % int(width) };

	const Rectf srcRect{  (Sector + 0) * m_ClipWidth, 0.f, m_ClipWidth, m_ClipHeight };
	const Rectf srcRect2{ (Sector + 1) * m_ClipWidth, 0.f, m_ClipWidth, m_ClipHeight };
	const Rectf srcRect3{ (Sector + 2) * m_ClipWidth, 0.f, m_ClipWidth, m_ClipHeight };
	const Rectf dstRect{  (Sector + 0) * width, 90.f, width ,viewport.height - 90.f };
	const Rectf dstRect2{ (Sector + 1) * width, 90.f, width ,viewport.height - 90.f };
	const Rectf dstRect3{ (Sector + 2) * width, 90.f, width ,viewport.height - 90.f };
	
	m_pBackgroundTexture->Draw(dstRect, srcRect);
	m_pBackgroundTexture->Draw(dstRect2, srcRect2);
	m_pBackgroundTexture->Draw(dstRect3, srcRect3);

	if (frame != -1 and (frame % 3 == 0 or frame % 4 == 0))
	{
		m_pBackgroundFullbright->Draw(dstRect, srcRect);
		m_pBackgroundFullbright->Draw(dstRect2, srcRect2);
		m_pBackgroundFullbright->Draw(dstRect3, srcRect3);
	}
}

void Level::DrawBackground(const Rectf viewport, const Rectf& avatar) const
{
	DrawBackground(viewport, avatar, -1);
}

bool Level::IsCollidingWithWorld(const Rectf& actorShape) const
{
	int extraWidth{ 0 };
	if (m_IsPlayerInBossRoom)
		extraWidth = m_ClipWidth;

	if ((m_IsPlayerLockedInBounds and actorShape.left <= m_Boundaries.width - (3 * m_ClipWidth / 4)) or actorShape.left + actorShape.width * 2 > m_Boundaries.width + extraWidth)  return true;

	utils::HitInfo hitInfo{};
	const Point2f beginHorBtm{ actorShape.left + actorShape.width, actorShape.bottom + 1 };
	const Point2f endHorBtmRight{ actorShape.left + actorShape.width * 2 + 2, actorShape.bottom + 1 }; // extending beam further to anticipate jump
	const Point2f endHorBtmLeft{ actorShape.left - 3, actorShape.bottom + 1 };
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		if (utils::Raycast(m_Vertices[i], beginHorBtm, endHorBtmRight, hitInfo) or utils::Raycast(m_Vertices[i], beginHorBtm, endHorBtmLeft, hitInfo))
		{
			return true;
		}
	}
	return false;
}

bool Level::IsCollidingVertically(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo{};
	const Point2f beginVert{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height };
	const Point2f endVertBtmRight{ actorShape.left + actorShape.width * 2 + 1, actorShape.bottom - 2 };
	const Point2f endVertBtmLeft{ actorShape.left - 1, actorShape.bottom - 2 };
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		if (utils::Raycast(m_Vertices[i], beginVert, endVertBtmRight, hitInfo) or utils::Raycast(m_Vertices[i], beginVert, endVertBtmLeft, hitInfo))
		{
			return true;
		}
	}
	return false;
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	float boundaryWidth{};
	if (m_IsPlayerInBossRoom) boundaryWidth = m_Boundaries.width + 3 * m_ClipWidth / 4;
	else boundaryWidth = m_Boundaries.width;

	if (actorShape.left <= 1) actorShape.left = 2.f;
	if (actorShape.left + actorShape.width * 2 > boundaryWidth and m_IsPlayerLockedInBounds) actorShape.left = boundaryWidth - actorShape.width * 2;
	if (m_IsPlayerLockedInBounds and actorShape.left <= boundaryWidth - (3 * m_ClipWidth / 4))
	{
		actorShape.left = boundaryWidth - (3 * m_ClipWidth / 4) + 1.f;
	}
	utils::HitInfo hitInfo{};
	const Point2f beginVertLeft{ actorShape.left, actorShape.bottom + actorShape.height * 2 };
	const Point2f beginVertRight{ actorShape.left + actorShape.width * 2, actorShape.bottom + actorShape.height * 2 };
	const Point2f endVertLeft{ actorShape.left, actorShape.bottom - 1};
	const Point2f endVertRight{ actorShape.left + actorShape.width * 2, actorShape.bottom - 1};

	const Point2f beginHorBtm{ actorShape.left + actorShape.width, actorShape.bottom + 2 };
	const Point2f endHorBtmRight{ actorShape.left + actorShape.width * 2 + 1, actorShape.bottom + 2};
	const Point2f endHorBtmLeft{ actorShape.left - 1, actorShape.bottom + 2};
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		if (utils::Raycast(m_Vertices[i], beginVertLeft, endVertLeft, hitInfo) or utils::Raycast(m_Vertices[i], beginVertRight, endVertRight, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y + 1;
			actorVelocity.y = 0;
			actorVelocity.x = 0;
		}
		if (utils::Raycast(m_Vertices[i], beginHorBtm, endHorBtmRight, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width * 2 - 1;
			actorVelocity.x = 0;
		}
		if (utils::Raycast(m_Vertices[i], beginHorBtm, endHorBtmLeft, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x + 1 ;
			actorVelocity.x = 0;
		}
	}
	
}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	utils::HitInfo hitInfo{};
	Point2f beginVertLeft{ actorShape.left, actorShape.bottom + actorShape.height * 2 };
	const Point2f beginVertRight{ actorShape.left + actorShape.width * 2, actorShape.bottom + actorShape.height * 2 };
	const Point2f endVertLeft{ actorShape.left, actorShape.bottom - 1 };
	const Point2f endVertRight{ actorShape.left + actorShape.width * 2, actorShape.bottom - 1 };
	for (int i = 0; i < m_Vertices.size(); i++) 
	{
		if (utils::Raycast(m_Vertices[i], beginVertLeft, endVertLeft, hitInfo) or utils::Raycast(m_Vertices[i], beginVertRight, endVertRight, hitInfo)) return true;
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::SetPlayerLockedInBounds()
{
	m_IsPlayerLockedInBounds = true;
}

void Level::ResetPlayerLockedInBounds()
{
	m_IsPlayerLockedInBounds = false;
}

void Level::SetPlayerInBossRoom()
{
	m_IsPlayerInBossRoom = true;
}