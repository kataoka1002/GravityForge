#include "stdafx.h"
#include "HumanEnemy.h"
#include "Game.h"
#include "IHumanEnemyState.h"
#include "HumanEnemyIdleState.h"

namespace
{
	//体力の最大値
	const float MAX_HP = 100.0f;
}

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemy::HumanEnemy()
	{
		//アニメーションの初期化
		animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Dead].Load("Assets/animData/enemy/humanEnemy_Dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Crawl].Load("Assets/animData/enemy/humanEnemy_Crawl.tka");
		animationClips[enAnimClip_Crawl].SetLoopFlag(true);
		animationClips[enAnimClip_Tremble].Load("Assets/animData/enemy/humanEnemy_Tremble.tka");
		animationClips[enAnimClip_Tremble].SetLoopFlag(true);
		animationClips[enAnimClip_Die].Load("Assets/animData/enemy/humanEnemy_die.tka");
		animationClips[enAnimClip_Die].SetLoopFlag(false); 
	}

	HumanEnemy::~HumanEnemy()
	{
		DeleteGO(m_collisionObject);
	}

	void HumanEnemy::InitModel()
	{
		m_model.Init("Assets/modelData/enemy/humanEnemy.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_model.SetPosition(m_position);
		m_model.SetRotation(m_rotation);
		m_model.SetScale(m_scale);
		m_model.Update();

		// キャラクターコントローラーを初期化
		m_charaCon.Init(
			30.0f,			//半径
			150.0f,			//高さ
			m_position		//座標
		);

		// HPの設定
		m_hp = MAX_HP;

		// 初期ステートを設定
		m_humanEnemyState = new HumanEnemyIdleState(this);
		m_humanEnemyState->Enter();
	}

	void HumanEnemy::Update()
	{
		// ステートを変更するか
		IHumanEnemyState* enemyState = m_humanEnemyState->StateChange();

		// 変更先のステートが設定されているならば
		if (enemyState != nullptr)
		{
			// ステートを変更する。
			delete m_humanEnemyState;
			m_humanEnemyState = enemyState;
			m_humanEnemyState->Enter();
		}

		// 各ステートの更新処理を実行。
		m_humanEnemyState->Update();

		// アニメーションを再生する。
		PlayAnimation(m_currentAnimationClip);

		// モデルを更新する。
		m_model.Update();
	}

	void HumanEnemy::OnDestroy()
	{
		//リストから削除
		m_game->RemoveEnemyFromList(this);

		//削除
		DeleteGO(this);
	}

	void HumanEnemy::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// アニメーションを再生
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void HumanEnemy::SetCollision()
	{
		//コリジョンオブジェクトを作成する。
		m_collisionObject = NewGO<CollisionObject>(0);
		Quaternion rot;
		rot.SetRotationDegX(90.0f);

		//カプセルのコリジョンを作成する。
		m_collisionObject->CreateCapsule(
			m_position,
			rot,
			30.0f,
			180.0f
		);

		//コリジョンに名前を付ける
		m_collisionObject->SetName("humanEnemy");

		//コリジョンオブジェクトが自動で削除されないようにする
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	void HumanEnemy::Render(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}