#include "stdafx.h"
#include "GameClear.h"
#include "ResultCamera.h"
#include "ResultUI.h"
#include "BlackFade.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	DeleteGO(m_resultCamera);
	DeleteGO(m_resultUI);
	DeleteGO(m_blackFade);
	DeleteGO(m_resultBGM);
}

void GameClear::InitCamera()
{
	//�J�����̐ݒ�
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameClear::InitUI()
{
	//UI�̐ݒ�
	m_resultUI = NewGO<ResultUI>(0, "resultui");
	m_resultUI->SetPlayerDead(false);
}

void GameClear::Update()
{
	//�^�C�g���֖߂�
	GoTitle();

	//�Đ�����A�j���[�V������ݒ�
	SetAnimation();

	if (m_pressAButton)
	{
		m_resultUI->SetFontAlpha();
	}

	// �A�j���[�V�������Đ�
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.5f);
	m_playerModel.Update();
}

void GameClear::OnDestroy()
{
	//���g�̍폜
	DeleteGO(this);
}

void GameClear::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
	m_bgModel.Draw(rc);
}