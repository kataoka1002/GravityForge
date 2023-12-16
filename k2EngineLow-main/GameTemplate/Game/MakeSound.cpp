#include "stdafx.h"
#include "MakeSound.h"

MakeSound::~MakeSound()
{

}

bool MakeSound::Start()
{
	//ƒTƒEƒ“ƒh‚ÌÝ’è
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/audio/titleBGM.wav");

	return true;
}

void MakeSound::Update()
{

}