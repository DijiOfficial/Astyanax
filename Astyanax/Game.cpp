#include "pch.h"
#include "Game.h"
#include <iostream>

Game::Game(const Window& window)
	:BaseGame{ window },
	m_GameState{ GameState::INTRO },
	//m_GameState{ GameState::WEAPONCHOICE },
	m_LastState{ GameState::INTRO }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	InitializeMusic();
	AddPowerUps();
	m_Level = new Level();
	m_Camera = new Camera{ GetViewPort().width, GetViewPort().height };
	m_Camera->SetLevelBoundaries(m_Level->GetBoundaries());
	m_Hud = new HUD{ Rectf{ 0, 0, GetViewPort().width, 90.f } };
	m_Lives = 2;
	m_Score = 0;
	m_CastingSpell = { false };
	m_PlayerDied = false;
	m_AnimTime = 0;
	m_NrFrames = 0;
	m_LockPLayer = false;
	m_IsCameraReset = false;
	m_GameIsPaused = false;
	m_UseVfx = true;
	m_Menu = new Menu{ Rectf{ 0, 0, GetViewPort().width, GetViewPort().height } };
	m_PauseMenu = new PauseMenu{ Rectf{ 0, 0, GetViewPort().width, GetViewPort().height } };
}

void Game::Cleanup( )
{
	delete m_Camera;
	m_Camera = nullptr;
	delete m_Hud;
	m_Hud = nullptr;
	ReleaseMusic();
	delete m_Menu;
	m_Menu = nullptr;
	delete m_Level;
	m_Level = nullptr;
	delete m_PauseMenu;
	m_PauseMenu = nullptr;
}

void Game::Update(const float elapsedSec )
{
	switch (m_GameState)
	{
	case Game::INTRO:
		m_Intro.Update(elapsedSec);
		if (m_Intro.GetIntroState()) 
		{
			delete g_pLevel1SoundTrack;
			g_pLevel1SoundTrack = new SoundStream("Audio/03 Title Screen.mp3");
			g_pLevel1SoundTrack->SetVolume(16);
			g_pLevel1SoundTrack->Play(true);
			m_GameState = MENU;
			m_Menu->SetActive();
		}
		break;
	case Game::MENU:
		m_Menu->Update(elapsedSec); 
		if (not m_Menu->GetMenuActive())
		{
			m_pStartSound->Play(0);
			delete g_pLevel1SoundTrack;
			g_pLevel1SoundTrack = new SoundStream("Audio/04 Intro _ Rounds 3-1 & 3-2.mp3");
			g_pLevel1SoundTrack->SetVolume(16);
			g_pLevel1SoundTrack->Play(true);
			m_GameState = ANIMATION1;
			m_FirstAnimation.SetActive();
		}
		break;
	case Game::ANIMATION1:
		m_FirstAnimation.Update(elapsedSec);
		if (m_FirstAnimation.GetFirstAnimationState())
		{
			delete g_pLevel1SoundTrack;
			g_pLevel1SoundTrack = new SoundStream("Audio/05 Round 1-1.mp3");
			g_pLevel1SoundTrack->SetVolume(16);
			g_pLevel1SoundTrack->Play(true);
			m_GameState = WEAPONCHOICE;
			m_WeaponChoice.SetUnselected();
		}
		break;

	case Game::WEAPONCHOICE:
		m_WeaponChoice.Update(elapsedSec);
		if (m_WeaponChoice.GetIsSelected())
		{
			m_Avatar.SetWeaponChoice(m_WeaponChoice.GetSelection());
			m_GameState = LEVEL1;
		}
		break;
	case Game::LEVEL1:
		UpdateLevel(elapsedSec);
		if (m_Avatar.GetState() == Avatar::ActionState::dead) 
		{
			m_pDeathSound->Play(0);
			m_Lives -= 1;
			delete m_Level;
			m_Level = new Level();
			m_Avatar.Reset();
		}
		break;
	case Game::PAUSE:
		break;
	default:
		break;
	}
}

void Game::Draw( ) const
{
	ClearBackground();

	switch (m_GameState)
	{
	case Game::INTRO:
		m_Intro.Draw();
		break;
	case Game::MENU:
		m_Menu->Draw();
		break;
	case Game::ANIMATION1:
		m_FirstAnimation.Draw();
		break;
	case Game::WEAPONCHOICE:
		m_WeaponChoice.Draw();
		break;
	case Game::LEVEL1:
		DrawGameObjects();
		break;
	case Game::PAUSE:
		switch (m_LastState)
		{
		case Game::INTRO:
			m_Intro.Draw();
			break;
		case Game::MENU:
			m_Menu->Draw();
			break;
		case Game::ANIMATION1:
			m_FirstAnimation.Draw();
			break;
		case Game::WEAPONCHOICE:
			m_WeaponChoice.Draw();
			break;
		case Game::LEVEL1:
			DrawGameObjects();
			break;
		default:
			break;
		}
		m_PauseMenu->Draw();
		break;
	default:
		break;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//	case SDLK_UP:
	//		m_Avatar.Update(elapsedSec, m_Level);
	//		break;
	//}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_w:
		m_Avatar.ChangePower();
		m_pPowerChange->Play(0);
		break;
	case SDLK_RETURN:
		if (m_Menu->GetMenuActive()) 
			m_Menu->StartGame();

		if (not m_Intro.GetIntroState()) 
			m_Intro.SkipIntro(); 

		if (not m_FirstAnimation.GetFirstAnimationState()) 
			m_FirstAnimation.SkipFirstAnimation();

		if (not m_WeaponChoice.GetIsSelected()) 
			m_WeaponChoice.SetSelected();

		if (m_GameState == GameState::PAUSE)
		{
			switch (m_PauseMenu->GetMenuCount())
			{
				case 0:
					m_pPause->Play(0);
					m_GameIsPaused = false;
					m_GameState = m_LastState;
					break;
				case 1:
					if (g_pLevel1SoundTrack->IsPlaying())
						g_pLevel1SoundTrack->Pause();
					break;
				case 2:
					g_pLevel1SoundTrack->Resume();
					break;
				case 3:
					m_UseVfx = !m_UseVfx;
					ToggleVfx();
					break;
				case 4:
					break;
				case 5:
					ResetGame();
					break;
				default:
					break;
			}
		}
		break;
	case SDLK_LEFT:
		if (m_WeaponChoice.GetSelection() > 0 and not m_WeaponChoice.GetIsSelected()) m_WeaponChoice.ChangeSelection(-1);
		break;
	case SDLK_RIGHT:
		if (m_WeaponChoice.GetSelection() < 2 and not m_WeaponChoice.GetIsSelected()) m_WeaponChoice.ChangeSelection(1);
		break;
	case SDLK_ESCAPE:
	case SDLK_i:
		m_pPause->Play(0);
		if (m_GameState != GameState::PAUSE)
		{
			m_LastState = m_GameState;
			m_GameState = GameState::PAUSE;
			m_GameIsPaused = true;
			DisplayControls();
			m_PauseMenu->ResetCount();
		}
		else
		{
			m_GameIsPaused = false;
			m_GameState = m_LastState;
		}
		break;
	case SDLK_F3:
		m_Debugger.m_ShowHitBox = !m_Debugger.m_ShowHitBox;
		break;
	case SDLK_g:
		m_Debugger.m_InfinitPower = !m_Debugger.m_InfinitPower;
		break;
	case SDLK_h:
		m_Debugger.m_OnePunch = !m_Debugger.m_OnePunch;
		break;
	case SDLK_UP:
		if (m_GameState == GameState::PAUSE)
			m_PauseMenu->DecrementMenuCount();
		break;
	case SDLK_DOWN:
		if (m_GameState == GameState::PAUSE)
			m_PauseMenu->IncrementMenuCount();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::DoCollisionTests()
{

	if (m_Avatar.IsAttacking()) m_EntityManager.HitEntity(m_Avatar.GetAttackZone(), m_Avatar.GetDamageDealt());
	if (m_EntityManager.HitAvatar(m_Avatar.GetShape())) m_Avatar.DealOneDamage(g_pPlayerHurt);
	if (m_Avatar.IsCastingSpell() and (not m_CastingSpell or m_Avatar.GetCurrentPower() == Powers::blast))
	{
		m_CastingSpell = true;
		m_EntityManager.CastSpellOnEntity(m_Avatar.GetCurrentPower(), m_Avatar.GetShape(), m_Avatar.GetSpellDamage(), m_Avatar.GetFireBallVector(), g_pTimeFreeze, m_pBlastAttack, m_pBoltAttack);
	}
	//add powerup
	if (m_EntityManager.GetStatueState())
	{
		const Point2f powerUpOffset{ 14, 25 };
		const Point2f statuePos{ m_EntityManager.GetStatuePos().x + powerUpOffset.x, m_EntityManager.GetStatuePos().y + powerUpOffset.y };
		m_PowerUpManager.AddItem(statuePos, m_Avatar.GetHealth(), m_Avatar.GetMana(), m_Avatar.GetMaxStrength());
	}
	if (m_PowerUpManager.HitItem(m_Avatar.GetShape()))
	{
		m_pPickUp->Play(0);
		m_Avatar.PowerUpHit(m_PowerUpManager.GetPowerType());
	}
}

void Game::AddPowerUps()
{
	const int statueId{ abs(int("statue")) };
	m_EntityManager.AddEntity(Point2f{ 1050, 154 }, statueId, Entity::EnemyType::powerUp, new Texture("statue.png"));
	m_EntityManager.AddEntity(Point2f{ 1910, 250 }, statueId, Entity::EnemyType::powerUp, new Texture("statue.png"));
	m_EntityManager.AddEntity(Point2f{ 3090, 186 }, statueId, Entity::EnemyType::powerUp, new Texture("statue.png"));
	m_EntityManager.AddEntity(Point2f{ 4015, 155 }, statueId, Entity::EnemyType::powerUp, new Texture("statue.png"));
}

void Game::InitializeMusic()
{
	g_pLevel1SoundTrack = new SoundStream("Audio/01 Opening - Part 1 _ Ending - Part 2.mp3");
	g_pLevel1SoundTrack->SetVolume(16);
	g_pLevel1SoundTrack->Play(true);
	g_pSwingSwoosh = new SoundEffect("Audio/SwingSwoosh.mp3");
	g_pEnemyDeath = new SoundEffect("Audio/EnemyDeathOld.mp3");
	g_pPlayerHurt = new SoundEffect("Audio/PlayerHurt.mp3");
	g_pTimeFreeze = new SoundEffect("Audio/FreezeTimeBleep.mp3");
	m_pBlastAttack = new SoundEffect("Audio/BlastAttack.mp3");
	m_pBoltAttack = new SoundEffect("Audio/BoltAttackShort.mp3");
	m_pPowerChange = new SoundEffect("Audio/PowerChange.mp3");
	m_pStartSound = new SoundEffect("Audio/StartSelect.mp3");
	m_pDeathSound = new SoundEffect("Audio/18 Game Over.mp3");
	m_pPickUp = new SoundEffect("Audio/pickup.mp3");
	m_pPause = new SoundEffect("Audio/pause.mp3");
	m_pPause->SetVolume(128);
	g_pEnemyDeath->SetVolume(32);
	g_pTimeFreeze->SetVolume(8);
	m_pBlastAttack->SetVolume(4);
	m_pBoltAttack->SetVolume(8);
	m_pPowerChange->SetVolume(8);
	m_pPickUp->SetVolume(128);

}

void Game::ReleaseMusic()
{
	delete g_pLevel1SoundTrack;
	g_pLevel1SoundTrack = nullptr;

	delete g_pSwingSwoosh;
	g_pSwingSwoosh = nullptr;

	delete g_pEnemyDeath;
	g_pEnemyDeath = nullptr;

	delete g_pPlayerHurt;
	g_pPlayerHurt = nullptr;

	delete g_pTimeFreeze;
	g_pTimeFreeze = nullptr;

	delete m_pBlastAttack;
	m_pBlastAttack = nullptr;

	delete m_pBoltAttack;
	m_pBoltAttack = nullptr;

	delete m_pPowerChange;
	m_pPowerChange = nullptr;

	delete m_pStartSound;
	m_pStartSound = nullptr;

	delete m_pDeathSound;
	m_pDeathSound = nullptr;

	delete m_pPickUp;
	m_pPickUp = nullptr;

	delete m_pPause;
	m_pPause = nullptr;
}

void Game::WalkToBossRoomAnimation()
{
	delete g_pLevel1SoundTrack;
	g_pLevel1SoundTrack = new SoundStream("Audio/06 Boss Intro.mp3");
	g_pLevel1SoundTrack->SetVolume(32);
	g_pLevel1SoundTrack->Play(0);

	const Rectf levelBoundaries{ m_Level->GetBoundaries() };
	m_Level->ResetPlayerLockedInBounds();
	if (m_IsCameraReset) m_Camera->UnlockCamera();
	m_LockPLayer = true;
	m_Avatar.ForceMovingState();
	if (m_Avatar.GetShape().left < levelBoundaries.width + GetViewPort().width / 3.f)
	{
		if (not m_IsCameraReset)
		{
			if (m_Avatar.GetShape().left < m_Camera->GetCameraPos(m_Avatar.GetShape()).left + m_Camera->GetCameraPos(m_Avatar.GetShape()).width / 2)
			{
				m_Avatar.ForceMoveRight();
			}
			else if (m_Avatar.GetShape().left - 2 > m_Camera->GetCameraPos(m_Avatar.GetShape()).left + m_Camera->GetCameraPos(m_Avatar.GetShape()).width / 2)
			{
				m_Camera->OffsetCamera(1);
			}
			else
			{
				m_IsCameraReset = true;
			}
		}
		else
		{
			m_Avatar.ForceMoveRight();
			m_Camera->OffsetCamera(1);
		}
	}
	else
	{
		if (m_Camera->GetCameraPos(m_Avatar.GetShape()).left < 4196)
		{
			m_Camera->OffsetCamera(1);
		}
		else
		{
			m_LockPLayer = false;
			Rectf newBounderies{ m_Level->GetBoundaries() };
			newBounderies.width += GetViewPort().width;
			m_Camera->SetLevelBoundaries(newBounderies);
			m_Level->SetPlayerInBossRoom();
			m_Camera->ResetOffset();
		}
	}
}

void Game::UpdateGameObjects(const float elapsedSec)
{
	m_EntityManager.Update(elapsedSec, m_Level, m_Avatar.GetShape(), g_pEnemyDeath);
	m_PowerUpManager.Update(elapsedSec, m_Level);
	m_Avatar.Update(elapsedSec, m_Level, g_pSwingSwoosh, m_LockPLayer);

	m_Score += m_EntityManager.GetScore();
	m_Hud->Update(m_Score, m_Lives, m_Avatar);

	//play boss music
	if (!g_pLevel1SoundTrack->IsPlaying())
	{
		delete g_pLevel1SoundTrack;
		g_pLevel1SoundTrack = new SoundStream("Audio/07 Boss Battle.mp3");
		g_pLevel1SoundTrack->SetVolume(32);
		g_pLevel1SoundTrack->Play(1);
	}
}

void Game::DrawGameObjects() const
{
	glPushMatrix();
	{
		m_Hud->Draw();
		m_Camera->Transform(m_Avatar.GetShape());
		if (m_AnimTime <= 2.f and m_CastingSpell and m_Avatar.GetCurrentPower() == Powers::bolt)
			m_Level->DrawBackground(GetViewPort(), m_Avatar.GetShape(), m_NrFrames);
		else
			m_Level->DrawBackground(GetViewPort(), m_Avatar.GetShape());
		m_EntityManager.Draw();
		m_PowerUpManager.Draw();
		m_Avatar.Draw();
	}
	glPopMatrix();
}

void Game::UpdateLevel(const float elapsedSec)
{
	if (m_EntityManager.GetMiniBossIsDead() and not (m_Avatar.GetShape().left > m_Level->GetBoundaries().width and not m_LockPLayer)) WalkToBossRoomAnimation();
	if (m_Camera->GetCameraPos(m_Avatar.GetShape()).left >= 3620 and not m_LockPLayer)
	{
		m_Level->SetPlayerLockedInBounds();
		m_Camera->LockCamera();
	}

	// Update game objects
	UpdateGameObjects(elapsedSec);

	// Do collision
	if (not m_Avatar.IsCastingSpell())
	{
		m_AnimTime = 0;
		m_CastingSpell = false;
	}
	if (m_CastingSpell)
	{
		m_AnimTime += elapsedSec;
		m_NrFrames += 1;
	}
	DoCollisionTests();
}

void Game::DisplayControls() const 
{
	std::cout << "Press I for Controls" << std::endl;
	std::cout << "Move Left and Righ with the Arrow Keys <- ->" << std::endl;
	std::cout << "Jump and crouch Using Up and Down Arrow Keys" << std::endl;
	std::cout << "Press 'x' to attack with your weapon" << std::endl;
	std::cout << "Press 'c' to cast a Spell" << std::endl;
	std::cout << "Press 'w' to Switch between Spells" << std::endl;
	std::cout << "Press 'enter' to skip Intro/Animation/Start Game" << std::endl;
	std::cout << "In Character selection menu Use Left and Right Arrow keys to navigate and Enter To validate your character" << std::endl;
	std::cout << "Press 'f3' for debug menu" << std::endl;
	std::cout << "Press 'g' for infinite Power" << std::endl;
	std::cout << "Press 'h' for infinite Strength" << std::endl;

}

void Game::ResetGame()
{
	m_pDeathSound->Play(0);
	
	m_Lives = 2;
	m_Score = 0;
	m_CastingSpell = { false };
	m_PlayerDied = false;
	m_AnimTime = 0;
	m_NrFrames = 0;
	m_LockPLayer = false;
	m_IsCameraReset = false;
	m_GameIsPaused = false;

	delete m_Level;
	m_Level = new Level();
	
	m_Avatar.Reset();
	m_WeaponChoice.SetUnselected();
	m_GameState = WEAPONCHOICE;
}

void Game::ToggleVfx()
{
	if (m_UseVfx)
	{
		g_pEnemyDeath->SetVolume(32);
		g_pTimeFreeze->SetVolume(8);
		m_pBlastAttack->SetVolume(4);
		m_pBoltAttack->SetVolume(8);
		m_pPowerChange->SetVolume(8);
		m_pPickUp->SetVolume(128);
	}
	else
	{
		g_pEnemyDeath->SetVolume(0);
		g_pTimeFreeze->SetVolume(0);
		m_pBlastAttack->SetVolume(0);
		m_pBoltAttack->SetVolume(0);
		m_pPowerChange->SetVolume(0);
		m_pPickUp->SetVolume(0);
	}
}
