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

	//タイトルへ戻る
	GoTitle();
}

void GameOver::Update()
{
	// アニメーションを再生
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.3f);
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