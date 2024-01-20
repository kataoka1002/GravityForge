#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "BlackFade.h"
#include "MakeSound.h"
#include "GameInformation.h"

bool Result::Start()
{
	//�{�XBGM�̒�~
	m_gameInfo = FindGO<GameInformation>("gameinformation");
	m_gameInfo->DeleteBossBGM();

	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Dwarf].Load("Assets/animData/player/player_dwarf.tka");
	animationClips[enAnimClip_Dwarf].SetLoopFlag(false);

	//�v���C���[���f���̏�����
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetPosition(m_playerPosition);
	m_playerModel.Update();

	//�w�i���f���̏�����
	m_bgModel.Init("Assets/modelData/map/map3.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bgModel.SetPosition(m_playerPosition);
	m_bgModel.Update();

	//������
	InitCamera();

	//UI�̍쐬
	InitUI();

	//�t�F�[�h�N���X�̍쐬
	m_blackFade = NewGO<BlackFade>(1, "blackfade");

	//BGM�̐ݒ�ƍĐ�
	m_resultBGM = NewGO<SoundSource>(0);
	m_resultBGM->Init(enSoundName_ResultBGM);						//������
	m_resultBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//���ʒ���
	m_resultBGM->Play(true);

	return true;
}

void Result::GoTitle()
{
	//A�{�^���Ń^�C�g���֖߂�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//A�{�^���������ꂽ��t�F�[�h�C��
		m_pressAButton = true;

		m_blackFade->SetAlphaUp(true);

		//���Đ�����ƏI���Ȃ̂�,�C���X�^���X��ێ������Ȃ��ׂɂ�����NewGO����
		SoundSource* clickSE = NewGO<SoundSource>(0);
		clickSE->Init(enSoundName_TitleClick);						//������
		clickSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//���ʒ���
		clickSE->Play(false);
	}

	if (m_pressAButton)
	{
		//�t�F�[�h�C�����I�������
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�^�C�g���̍쐬
			NewGO<Title>(0, "title");

			//���ʂ̃��Z�b�g
			g_soundEngine->SetBgmAndSeVolume(1.0f, 1.0f);

			//�폜����
			OnDestroy();
		}
	}
}

void Result::SetAnimation()
{
	//���Ԃ̌o�߂𑪂�
	m_time += g_gameTime->GetFrameDeltaTime();

	//2�b�o�߂�
	if (m_time >= 2.0f)
	{
		//1�b�o�߂Ń��[�V�����Đ�
		m_currentAnimationClip = enAnimClip_Dwarf;

		//�h���[�t�A�j���Đ��ς݂ɂ���
		m_didPlayAnim = true;
	}

	//�h���[�t�A�j�����Đ����I�������
	if (m_didPlayAnim && m_playerModel.IsPlayingAnimation() == false)
	{
		//�A�C�h���𗬂�
		m_currentAnimationClip = enAnimClip_Idle;

		//�h���[�t�A�j�����Đ���Ԃɖ߂�
		m_didPlayAnim = false;

		//�^�C�}�[�����Z�b�g
		m_time = 0.0f;
	}
}