#pragma once
#include "utils.h"
#include "Texture.h"

class HitMarker
{
public:
	explicit HitMarker(const Point2f& bottomLeft);
	~HitMarker();
	HitMarker(const HitMarker& other) = delete;
	HitMarker(HitMarker&& other) noexcept = delete;
	HitMarker& operator=(const HitMarker& other) = delete;
	HitMarker& operator=(HitMarker&& other) noexcept = delete;
	void Draw() const;
	void Update(float elapsedSec);
	int GetCurrentAnimFrame() const;
private:	
	const Texture* m_pHitTexture;
	float m_AnimTime;
	int m_AnimFrame;
	const Point2f m_BottomLeft;
};

