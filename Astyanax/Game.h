#pragma once
#include "BaseGame.h"
#include "Level.h"
#include "Avatar.h"
#include "Camera.h"
#include "HUD.h"
#include "PowerUpManager.h"
#include "EntityManager.h"
#include <SoundStream.h>
#include "SoundEffect.h"
#include "Intro.h"
#include "TextureManager.h"
#include "Menu.h"
#include "FirstAnimation.h"
#include "Weapon.h"
#include "PauseMenu.h"
#include "FirstBoss.h"

class Scoreboard;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game() override;

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// DATA MEMBERS
	Level* m_Level;
	Avatar* m_Avatar;
	Camera* m_Camera;
	HUD* m_Hud;
	PowerUpManager m_PowerUpManager;
	EntityManager m_EntityManager;
	PauseMenu* m_PauseMenu;
	Scoreboard* m_Scoreboard;

	int m_Score;
	int m_Lives;
	bool m_CastingSpell;
	float m_AnimTime;
	int m_NrFrames;
	bool m_LockPLayer;
	bool m_IsCameraReset;
	bool m_PlayerDied;
	bool m_GameIsPaused;
	bool m_UseVfx;

	std::string m_PlayerName{""};
	std::string m_LetterInput{""};

	int m_RemainingHealth{ 0 };
	int m_RemainingMana{ 0 };
	Intro m_Intro;
	Menu* m_Menu;
	FirstAnimation m_FirstAnimation;
	Weapon m_WeaponChoice;
	Debug m_Debugger;
	const enum GameState
	{
		INTRO,
		MENU,
		ANIMATION1,
		WEAPONCHOICE,
		LEVEL1,
		PAUSE,
		SCOREBOARD,
		ROUNDOVER,
		GAMEOVER,
	};
	GameState m_GameState;
	GameState m_LastState;

	//AUDIO
	SoundStream* g_pLevel1SoundTrack;
	SoundEffect* g_pSwingSwoosh;
	SoundEffect* g_pEnemyDeath;
	SoundEffect* g_pPlayerHurt;
	SoundEffect* g_pTimeFreeze;
	SoundEffect* m_pBlastAttack;
	SoundEffect* m_pBoltAttack;
	SoundEffect* m_pPowerChange;
	SoundEffect* m_pStartSound;
	SoundEffect* m_pDeathSound;
	SoundEffect* m_pPickUp;
	SoundEffect* m_pPause;
	SoundEffect* m_pBonusPointUp;
	SoundEffect* m_pBonusPointDown;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void DoCollisionTests();
	void AddPowerUps();

	void InitializeMusic();
	void ReleaseMusic();

	void WalkToBossRoomAnimation();
	void UpdateGameObjects(float elapsedSec);
	void DrawGameObjects() const;
	void UpdateLevel(float elapsedSec);

	void DisplayControls() const;
	void ResetGame();
	void ToggleVfx();

	void DrawFillPlayerName() const;
	void UpdateGameOver(float elapsedSec);

	void DrawLevelClear() const;
	void UpdateLevelClear(float elapsedSec);
	void DeadBossUpdate();

};

