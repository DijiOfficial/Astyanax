#pragma once
#include <vector>
#include "utils.h"
#include "Texture.h"
class Level
{
public:
	Level();
	~Level();
	Level(const Level& other) = delete;
	Level(Level&& other) noexcept = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other) noexcept = delete;
	void DrawBackground(const Rectf viewport, const Rectf& avatar, const int frame) const;
	void DrawBackground(const Rectf viewport, const Rectf& avatar) const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;
	bool IsCollidingWithWorld(const Rectf& actorShape) const;
	bool IsCollidingVertically(const Rectf& actorShape) const;
	Rectf GetBoundaries() const;
	void SetPlayerLockedInBounds();
	void ResetPlayerLockedInBounds();
	void SetPlayerInBossRoom();
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	const Texture* m_pBackgroundTexture;
	const Texture* m_pBackgroundFullbright;
	Rectf m_Boundaries;
	const float m_ClipHeight;
	const float m_ClipWidth;
	bool m_IsPlayerLockedInBounds;
	bool m_IsPlayerInBossRoom;
};

