#include "stdafx.h"
#include "HumanEnemy.h"
#include "Game.h"

namespace
{
	//体力の最大値
	const float MAX_HP = 100.0f;
}

HumanEnemy::HumanEnemy()
{
	//アニメーションの初期化
	animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
}

HumanEnemy::~HumanEnemy()
{

}

void HumanEnemy::InitModel()
{
	m_model.Init("Assets/modelData/enemy/humanEnemy.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		30.0f,			//半径
		150.0f,			//高さ
		m_position		//座標
	);

	//HPの設定
	m_hp = MAX_HP;
}

void HumanEnemy::Update()
{
	if (m_hp <= 0.0f)
	{
		OnDestroy();
	}

	switch (m_humanEnemyState)
	{
		//プレイヤーステートが待機だったら
	case enHumanEnemyState_Idle:
		//待機アニメーションを再生
		m_model.PlayAnimation(enAnimClip_Idle, 0.5f);
		break;

	default:
		break;
	}
	m_model.Update();
}

void HumanEnemy::OnDestroy()
{
	m_game->RemoveEnemyFromList(this);
	DeleteGO(this);
}

void HumanEnemy::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
