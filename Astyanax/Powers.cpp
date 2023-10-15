#include "pch.h"
#include "Powers.h"
#include "FireBall.h"

Powers::Powers(const Power power) :
	m_CurrentPower{ power }	
{

}

Powers::~Powers()
{
	for (int i = 0; i < m_pFireballs.size(); i++)
	{
		delete m_pFireballs[i];
		m_pFireballs[i] = nullptr;
	}
}

void Powers::Draw() const
{
	if (!m_pFireballs.empty())
	{
		for (int i = 0; i < m_pFireballs.size(); i++)
		{
			m_pFireballs[i]->Draw();
		}
	}
}

void Powers::Update(const float elapsedSec)
{
	if (!m_pFireballs.empty())
	{
		for (int i = 0; i < m_pFireballs.size(); i++)
		{
			m_pFireballs[i]->Update(elapsedSec);
		}
		for (int i = 0; i < m_pFireballs.size(); i++)
		{
			if (m_pFireballs[i]->GetDistance() >= 400)
			{
				delete m_pFireballs[i];
				m_pFireballs[i] = m_pFireballs[m_pFireballs.size() - 1];
				m_pFireballs.pop_back();
			}
		}
	}
}

void Powers::ChangePower()
{
	switch (m_CurrentPower)
	{
	case bolt:
		m_CurrentPower = blast;
		break;
	case blast:
		m_CurrentPower = bind;
		break;
	case bind:
		m_CurrentPower = bolt;
		break;
	default:
		break;
	}
}

Powers::Power Powers::GetCurrentPower() const
{
	return m_CurrentPower;
}

void Powers::AddFireballs(const Rectf avatar)
{
	constexpr float fireballSpeed{ 400.f };
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{		fireballSpeed,			0.f }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ 3 * fireballSpeed / 4.f,	3 * fireballSpeed / 4.f }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ 0.f,							fireballSpeed }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ -3 * fireballSpeed / 4.f,	3 * fireballSpeed / 4.f }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ -fireballSpeed,			0.f }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ -3 * fireballSpeed / 4.f,	-3 * fireballSpeed / 4.f }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ 0.f,							-fireballSpeed }));
	m_pFireballs.push_back(new FireBall(Point2f{ avatar.left + avatar.width / 2, avatar.bottom + avatar.height / 2 }, Vector2f{ 3 * fireballSpeed / 4.f,	-3 * fireballSpeed / 4.f }));

}

std::vector<FireBall*>& Powers::GetFireBallVector()
{
	return m_pFireballs;
}

