#include "stdafx.h"
#include "GameOver.h"
#include "ResultCamera.h"
#include "ResultUI.h"
#include "BlackFade.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	DeleteGO(m_resultCamera);
	DeleteGO(m_resultUI);
	DeleteGO(m_blackFade);
	DeleteGO(m_resultBGM);
}

void GameOver::InitCamera()
{
	//�J�����̐ݒ�
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameOver::InitUI()
{
	//UI�̐ݒ�
	m_resultUI = NewGO<ResultUI>(0, "resultui");
	m_resultUI->SetPlayerDead(true);
}

void GameOver::Update()
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

void GameOver::OnDestroy()
{
	//���g�̍폜
	DeleteGO(this);
}

void GameOver::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
	m_bgModel.Draw(rc);
}