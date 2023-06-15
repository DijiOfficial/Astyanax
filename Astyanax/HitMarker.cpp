#include "pch.h"
#include "HitMarker.h"


HitMarker::HitMarker(const Point2f& bottomLeft) :
	m_pHitTexture{ new Texture {"hit.png"} },
	m_AnimTime{},
	m_AnimFrame{},
	m_BottomLeft{bottomLeft}
{

}

HitMarker::~HitMarker()
{
	delete m_pHitTexture;
	m_pHitTexture = nullptr;
}

void HitMarker::Draw() const
{
	const Rectf srcRect{ m_AnimFrame * 96.f , 0.f, 96.f, 87.f };
	const Rectf dstRect{ m_BottomLeft.x, m_BottomLeft.y , 30.f, 30.f };
	m_pHitTexture->Draw(dstRect, srcRect);
}

void HitMarker::Update(const float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 0.5f / 8.f)
	{
		++m_AnimFrame;
		m_AnimTime -= 0.5f / 8.f;
	}
}

int HitMarker::GetCurrentAnimFrame() const
{
	return m_AnimFrame;
}
