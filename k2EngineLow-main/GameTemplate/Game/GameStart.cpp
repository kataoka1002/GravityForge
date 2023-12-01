#include "stdafx.h"
#include "GameStart.h"
#include "GameInformation.h"

namespace
{
	const float ALPHA_UP_SPEED = 0.1f;

	const float ALPHA_DOWN_SPEED = 0.05f;

	const float SPRITE_DELETE_TIME = 1.0f;
}

GameStart::~GameStart()
{

}

bool GameStart::Start()
{
	//インフォメーションのインスタンスを探す。
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//画像の初期化
	m_gameStartSprite.Init("Assets/spriteData/BattleStart!.dds", 1463.0f, 759.0f);
	m_gameStartSprite.SetScale(1.08f);
	m_gameStartSprite.Update();

	return true;
}

void GameStart::Update()
{
	//透明度アップの時
	if (m_alphaUP == true)
	{
		m_alpha += ALPHA_UP_SPEED;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_alphaUP = false;
		}
	}
	//透明度ダウンの時
	else if (m_alphaDown == true)
	{
		m_alpha -= ALPHA_DOWN_SPEED;
		if (m_alpha <= 0.0f)
		{
			//ゲームスタート
			m_gameInfo->SetInGame(true);

			DeleteGO(this);
		}
	}
	//どちらでもないとき
	else
	{
		//時間計測
		m_timer += g_gameTime->GetFrameDeltaTime();

		//一定時間経過でダウンにする
		if (m_timer >= SPRITE_DELETE_TIME)
		{
			m_alphaDown = true;
		}
	}

	m_gameStartSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_alpha });
	m_gameStartSprite.Update();
}

void GameStart::Render(RenderContext& rc)
{
	m_gameStartSprite.Draw(rc);
}
