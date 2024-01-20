#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "BlackFade.h"
#include "MakeSound.h"
#include "GameInformation.h"

bool Result::Start()
{
	//ボスBGMの停止
	m_gameInfo = FindGO<GameInformation>("gameinformation");
	m_gameInfo->DeleteBossBGM();

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

	//BGMの設定と再生
	m_resultBGM = NewGO<SoundSource>(0);
	m_resultBGM->Init(enSoundName_ResultBGM);						//初期化
	m_resultBGM->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//音量調整
	m_resultBGM->Play(true);

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

		//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
		SoundSource* clickSE = NewGO<SoundSource>(0);
		clickSE->Init(enSoundName_TitleClick);						//初期化
		clickSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
		clickSE->Play(false);
	}

	if (m_pressAButton)
	{
		//フェードインが終わったら
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//タイトルの作成
			NewGO<Title>(0, "title");

			//音量のリセット
			g_soundEngine->SetBgmAndSeVolume(1.0f, 1.0f);

			//削除処理
			OnDestroy();
		}
	}
}

void Result::SetAnimation()
{
	//時間の経過を測る
	m_time += g_gameTime->GetFrameDeltaTime();

	//2秒経過で
	if (m_time >= 2.0f)
	{
		//1秒経過でモーション再生
		m_currentAnimationClip = enAnimClip_Dwarf;

		//ドワーフアニメ再生済みにする
		m_didPlayAnim = true;
	}

	//ドワーフアニメが再生し終わったら
	if (m_didPlayAnim && m_playerModel.IsPlayingAnimation() == false)
	{
		//アイドルを流す
		m_currentAnimationClip = enAnimClip_Idle;

		//ドワーフアニメ未再生状態に戻す
		m_didPlayAnim = false;

		//タイマーをリセット
		m_time = 0.0f;
	}
}