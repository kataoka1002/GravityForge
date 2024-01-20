#include "stdafx.h"
#include "MakeSound.h"

MakeSound::~MakeSound()
{

}

bool MakeSound::Start()
{
	//ƒTƒEƒ“ƒh‚ÌÝ’è
	g_soundEngine->ResistWaveFileBank(enSoundName_GameBGM, "Assets/audio/GameBGM.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossBGM, "Assets/audio/bossBGM.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_ResultBGM, "Assets/audio/resultBGM.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/audio/titleBGM.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleClick, "Assets/audio/titleClick.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleCancel, "Assets/audio/titleCancel.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleMove, "Assets/audio/titleMove2.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_OpDistortion, "Assets/audio/distortion.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_ObjBomb, "Assets/audio/bombMini.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_ObjBlowWind, "Assets/audio/objectWindnew.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_ObjUpWind, "Assets/audio/objectUpWind.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossName, "Assets/audio/bossName.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossJumpAttack, "Assets/audio/bossJumpAttack.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossWalk, "Assets/audio/bossWalk.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_HumanEnemyAttack, "Assets/audio/humanEnemyAttack.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossSlash, "Assets/audio/bossSlash.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_BossPunch, "Assets/audio/bossPunch.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_PlayerPunch, "Assets/audio/playerPunch.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_PlayerWalk, "Assets/audio/playerWalk.wav");

	return true;
}

void MakeSound::Update()
{

}