#include "stdafx.h"
#include "HumanEnemy.h"

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
}

void HumanEnemy::Update()
{
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

void HumanEnemy::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
