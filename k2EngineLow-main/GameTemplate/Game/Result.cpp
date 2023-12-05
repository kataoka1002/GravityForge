#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "BlackFade.h"

bool Result::Start()
{
	//アニメーションの初期化
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Dwarf].Load("Assets/animData/player/player_dwarf.tka");
	animationClips[enAnimClip_Dwarf].SetLoopFlag(false);

	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetPosition(m_playerPosition);
	m_playerModel.Update();

	//背景モデルの初期化
	m_bgModel.Init("Assets/modelData/map/map3.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bgModel.SetPosition(m_playerPosition);
	m_bgModel.Update();

	//初期化
	InitCamera();

	//UIの作成
	InitUI();

	//フェードクラスの作成
	m_blackFade = NewGO<BlackFade>(1, "blackfade");

	return true;
}

void Result::GoTitle()
{
	//Aボタンでタイトルへ戻る
	if (g_pad[0]->IsPress(enButtonA))
	{
		//Aボタンが押されたらフェードイン
		m_pressAButton = true;

		m_blackFade->SetAlphaUp(true);
	}

	if (m_pressAButton)
	{
		//フェードインが終わったら
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//タイトルの作成
			NewGO<Title>(0, "title");

			//削除処理
			OnDestroy();
		}
	}
}

void Result::SetAnimation()
{
	//ドワーフのアニメーションが再生されているなら
	if (m_didPlayAnim == true)
	{
		return;
	}

	//時間の経過を測る
	m_time += g_gameTime->GetFrameDeltaTime();

	if (m_time >= 1.0f)
	{
		//1秒経過でモーション再生
		m_currentAnimationClip = enAnimClip_Dwarf;
	}

	//アニメーションが終わったら
	if (m_playerModel.IsPlayingAnimation() == false)
	{
		//アイドルを流す
		m_currentAnimationClip = enAnimClip_Idle;

		//ドワーフアニメ再生済みにする
		m_didPlayAnim = true;
	}
}