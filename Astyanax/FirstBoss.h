#pragma once
#include "Entity.h"

class BossBody;
class BossHead;

class FirstBoss final : public Entity
{
public:
	FirstBoss(const Point2f& bottomLeft, const int id, const Entity::EnemyType enemyType, const Texture* texture);
	~FirstBoss();
	FirstBoss(const FirstBoss& other) = delete;
	FirstBoss(FirstBoss&& other) noexcept = delete;
	FirstBoss& operator=(const FirstBoss& other) = delete;
	FirstBoss& operator=(FirstBoss&& other) noexcept = delete;

	void Draw() const override;
	void Update(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio) override;
	
	void KillEntity() override;
	ActionState GetState() const override;
	bool GetIsFirstPhaseOver() const override { return !m_SwitchPhase; };

private:
	BossBody* m_BossBody{};
	BossHead* m_BossHead{};
	Point2f m_BottomLeft;
	
	Rectf m_TextClip;

	float m_IdleTime;
	float m_AttackTime;
	bool m_IsAttacking;

	bool m_SwitchPhase;
	bool m_PlayFirstPhaseDeathAnimation;

	Entity::ActionState m_ActionState;
	void UpdateFrames(const float elapsedSec) override;
	void CheckForDesync();

	void FirstPhase(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio);
	void SecondPhase(float elapsedSec, Level*& level, const Rectf& avatar, SoundEffect* deathAudio);

	//void DrawHealthBar() const; //potential idea
	void KillFirstPhaseEntities() const;
	void CreateSecondPhase();
	void DrawMainBody() const;
};

