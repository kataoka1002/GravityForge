#include "stdafx.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyReactionState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyAttackState::~HumanEnemyAttackState()
	{

	}

	void HumanEnemyAttackState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Attack, 0.2f);
		//アニメーション速度を上げる
		m_enemy->SetAnimationSpeed(2.0f);
	}

	IHumanEnemyState* HumanEnemyAttackState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//アニメーション速度をもとにもどす
			m_enemy->SetAnimationSpeed(1.0f);

			return new HumanEnemyIdleState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		//回転処理
		m_enemy->TurnWhileAttack();
	}
}
