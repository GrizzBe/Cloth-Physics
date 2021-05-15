// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   AudioManager.cpp
//  Description :   Manages audio.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "AudioManager.h"
 // Static Variables 
CAudioManager* CAudioManager::sm_Instance = 0;
 // Implementation 
CAudioManager::CAudioManager()
{
	m_bMusicPlaying = false;

	m_AudioSystem = 0;
	m_FX_PlayerDmg = 0;
	m_FX_EnemyDeath = 0;
	m_FX_Fire = 0;
	m_FX_StartWave = 0;
	m_Track_Background = 0;
}

CAudioManager::~CAudioManager()
{
	m_Track_Background->release();
	m_FX_PlayerDmg->release();
	m_FX_EnemyDeath->release();
	m_FX_Fire->release();
	m_FX_StartWave->release();
	m_AudioSystem->release();
}

/***********************
* GetInstance: Returns instance of AudioManager.
* @author: William de Beer
* @return: Return AudioManager
********************/
CAudioManager& CAudioManager::GetInstance()
{
	if (sm_Instance == 0)
	{
		sm_Instance = new CAudioManager();
		sm_Instance->AudioInit();
	}
	return *sm_Instance;
}

/***********************
* RemoveInstance: Deletes current instance of AudioManager.
* @author: William de Beer
********************/
void CAudioManager::RemoveInstance()
{
	if (sm_Instance != 0)
	{
		delete sm_Instance;
		sm_Instance = 0;
	}
}

/***********************
* PlaySound: Plays Specified Sound.
* @author: William de Beer
* @parameter: Sound enum
********************/
void CAudioManager::PlaySound(Sounds _soundname)
{
	FMOD_RESULT Result;
	switch (_soundname) // Check which sound is to be played
	{
	case Sounds::PlayerDamage: // Play recieve damage sound
		Result = m_AudioSystem->playSound(m_FX_PlayerDmg, 0, false, 0);
		break;
	case Sounds::EnemyDeath: // Play kill enemy sound
		Result = m_AudioSystem->playSound(m_FX_EnemyDeath, 0, false, 0);
		break;
	case Sounds::Fire: // Play fire weapon sound
		Result = m_AudioSystem->playSound(m_FX_Fire, 0, false, 0);
		break;
	case Sounds::StartWave: // Play start wave
		Result = m_AudioSystem->playSound(m_FX_StartWave, 0, false, 0);
		break;
	case Sounds::Music1: // Play Music1 track
		if (!m_bMusicPlaying) // Check if it's already playing.
		{
			Result = m_AudioSystem->playSound(m_Track_Background, 0, false, 0);
			m_bMusicPlaying = true;
		}
		break;
	}
}

/***********************
* AudioInit: Initialise audio system and sounds.
* @author: William de Beer
* @return: Return boolean
********************/
bool CAudioManager::AudioInit()
{
	FMOD_RESULT Result;
	
	// Initialise Audio Systems
	if (FMOD::System_Create(&m_AudioSystem) != FMOD_OK)
	{
		return false;
	}
	
	if (m_AudioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED ,0))
	{
		return false;
	}
	
	// Create sounds
	Result = m_AudioSystem->createSound("Resources/Audio/PlayerDamage.wav", FMOD_DEFAULT, 0, &m_FX_PlayerDmg);
	Result = m_AudioSystem->createSound("Resources/Audio/EnemyDeath.wav", FMOD_DEFAULT, 0, &m_FX_EnemyDeath);
	Result = m_AudioSystem->createSound("Resources/Audio/Fire.wav", FMOD_DEFAULT, 0, &m_FX_Fire);
	Result = m_AudioSystem->createSound("Resources/Audio/StartWave.wav", FMOD_DEFAULT, 0, &m_FX_StartWave);
	Result = m_AudioSystem->createSound("Resources/Audio/BGmusic.mp3", FMOD_LOOP_NORMAL, 0, &m_Track_Background);
	
	return true;
}

/***********************
* Process: Updates audio system.
* @author: William de Beer
********************/
void CAudioManager::Process()
{
	m_AudioSystem->update();
}

