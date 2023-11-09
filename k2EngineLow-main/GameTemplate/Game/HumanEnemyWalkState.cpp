#include "stdafx.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyAttackState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyWalkState::~HumanEnemyWalkState()
	{

	}

	void HumanEnemyWalkState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Walk, 0.5f);
	}

	IHumanEnemyState* HumanEnemyWalkState::StateChange()
	{
		// 体力が50以下で四つん這い
		if (m_enemy->GetHP() <= KNEELING_HP)
		{
			return new HumanEnemyDeadState(m_enemy);
		}

		//一定距離以上になると追いかけてこなくなる
		if (m_enemy->CheckDistanceToPlayer() >= CHASE_RANGE)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		//一定距離以下で攻撃
		if (m_enemy->CheckDistanceToPlayer() <= ATTACK_RANGE)
		{
			return new HumanEnemyAttackState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyWalkState::Update()
	{
		//プレイヤーを追いかける
		m_enemy->FollowPlayer();

		//回転処理
		m_enemy->Turn();
	}
}
