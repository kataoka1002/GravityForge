#include "stdafx.h"
#include "MakeSound.h"

MakeSound::~MakeSound()
{

}

bool MakeSound::Start()
{
	//サウンドの設定
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/audio/titleBGM.wav");

	return true;
}

void MakeSound::Update()
{

}