#include "stdafx.h"
#include "GameClear.h"
#include "ResultCamera.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	DeleteGO(m_resultCamera);
}

void GameClear::InitCamera()
{
	//カメラの設定
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameClear::Update()
{
	//タイトルへ戻る
	GoTitle();

	//再生するアニメーションを設定
	SetAnimation();

	// アニメーションを再生
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.5f);
	m_playerModel.Update();
}

void GameClear::OnDestroy()
{
	//自身の削除
	DeleteGO(this);
}

void GameClear::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}