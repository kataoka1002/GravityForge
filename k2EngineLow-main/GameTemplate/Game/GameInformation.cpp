#include "stdafx.h"
#include "GameInformation.h"
#include "MakeSound.h"

namespace
{
	const int ENABLE_BOSSBATTLE_ENEMY_NUM = 1;
}

GameInformation::~GameInformation()
{
	DeleteGO(m_gameBGM);
	DeleteGO(m_zombieVoice);
}

void GameInformation::DeleteBossBGM()
{
	m_bossBGM->Stop();
	DeleteGO(m_bossBGM);
}

bool GameInformation::Start()
{
	//BGMの設定と再生
	m_gameBGM = NewGO<SoundSource>(0);
	m_gameBGM->Init(enSoundName_GameBGM);						//初期化
	m_gameBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//音量調整
	m_gameBGM->Play(true);

	//BGMの設定と再生
	m_bossBGM = NewGO<SoundSource>(0);
	m_bossBGM->Init(enSoundName_BossBGM);						//初期化
	m_bossBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//音量調整

	//BGMの設定と再生
	m_zombieVoice = NewGO<SoundSource>(0);
	m_zombieVoice->Init(enSoundName_ZombieVoice);					//初期化
	m_zombieVoice->SetVolume(0.5f * g_soundEngine->GetBgmVolume());	//音量調整
	m_zombieVoice->Play(true);

	return true;
}

void GameInformation::Update()
{
	//ボス戦なら
	if (m_isInBossBattle)
	{
		//ボス撃破時間の計測
		m_bossKillTime += g_gameTime->GetFrameDeltaTime();

		//ボスBGMが再生されていないなら
		if (m_bossBGM->IsPlaying() == false)
		{
			m_gameBGM->Stop();
			m_zombieVoice->Stop();
			m_bossBGM->Play(true);
		}
	}
}

void GameInformation::StopBGM()
{
	m_gameBGM->Stop();
	m_zombieVoice->Stop();
	m_bossBGM->Stop();
}