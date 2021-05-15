#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   AudioManager.h
//  Description :   Manages audio.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <fmod.hpp>
#include <map>
#include <string>
 // Implementation 
enum class Sounds
{
	PlayerDamage,
	EnemyDeath,
	Fire,
	StartWave,
	Music1,
};
class CAudioManager
{
public:

	~CAudioManager();

	static CAudioManager& GetInstance();
	static void RemoveInstance();

	void PlaySound(Sounds _soundname);
	bool AudioInit();
	void Process();

private:
	CAudioManager();

	static CAudioManager* sm_Instance;

	bool m_bMusicPlaying;

	FMOD::System* m_AudioSystem;

	// Sounds to be loaded.
	FMOD::Sound* m_FX_PlayerDmg;
	FMOD::Sound* m_FX_EnemyDeath;
	FMOD::Sound* m_FX_Fire;
	FMOD::Sound* m_FX_StartWave;

	// Music tracks to be loaded.
	FMOD::Sound* m_Track_Background;
};
#endif 
