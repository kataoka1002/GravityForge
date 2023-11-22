#include "stdafx.h"
#include "GameOver.h"
#include "ResultCamera.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	DeleteGO(m_resultCamera);
}

void GameOver::InitCamera()
{
	//カメラの設定
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameOver::Update()
{
	//タイトルへ戻る
	GoTitle();

	//再生するアニメーションを設定
	SetAnimation();

	// アニメーションを再生
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.5f);
	m_playerModel.Update();
}

void GameOver::OnDestroy()
{
	//自身の削除
	DeleteGO(this);
}

void GameOver::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}