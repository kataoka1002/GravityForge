#include "stdafx.h"
#include "MakeSound.h"

MakeSound::~MakeSound()
{

}

bool MakeSound::Start()
{
	//サウンドの設定
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/audio/titleBGM.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleClick, "Assets/audio/titleClick.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleCancel, "Assets/audio/titleCancel.wav");
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleMove, "Assets/audio/titleMove2.wav");

	return true;
}

void MakeSound::Update()
{

}