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
	//BGM�̐ݒ�ƍĐ�
	m_gameBGM = NewGO<SoundSource>(0);
	m_gameBGM->Init(enSoundName_GameBGM);						//������
	m_gameBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//���ʒ���
	m_gameBGM->Play(true);

	//BGM�̐ݒ�ƍĐ�
	m_bossBGM = NewGO<SoundSource>(0);
	m_bossBGM->Init(enSoundName_BossBGM);						//������
	m_bossBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//���ʒ���

	//BGM�̐ݒ�ƍĐ�
	m_zombieVoice = NewGO<SoundSource>(0);
	m_zombieVoice->Init(enSoundName_ZombieVoice);					//������
	m_zombieVoice->SetVolume(0.5f * g_soundEngine->GetBgmVolume());	//���ʒ���
	m_zombieVoice->Play(true);

	return true;
}

void GameInformation::Update()
{
	//�{�X��Ȃ�
	if (m_isInBossBattle)
	{
		//�{�X���j���Ԃ̌v��
		m_bossKillTime += g_gameTime->GetFrameDeltaTime();

		//�{�XBGM���Đ�����Ă��Ȃ��Ȃ�
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