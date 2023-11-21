#include "stdafx.h"
#include "Result.h"
#include "Title.h"

bool Result::Start()
{
	//アニメーションの初期化
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);

	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetPosition(m_playerPosition);
	m_playerModel.Update();

	//初期化
	InitCamera();

	return true;
}

void Result::GoTitle()
{
	//Aボタンでタイトルへ戻る
	if (g_pad[0]->IsPress(enButtonA))
	{
		//タイトルの作成
		NewGO<Title>(0, "title");

		//削除処理
		OnDestroy();
	}
}