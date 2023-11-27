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
	}

	IHumanEnemyState* HumanEnemyAttackState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		////攻撃を受けたら
		//if (m_enemy->DidAttackHit())
		//{
		//	// 体力が一定以下で四つん這い
		//	if (m_enemy->GetHP() <= KNEELING_HP)
		//	{
		//		//体力0以下で死亡
		//		return new HumanEnemyDeadState(m_enemy);
		//	}
		//	else
		//	{
		//		//体力あるならリアクションのみ
		//		return new HumanEnemyReactionState(m_enemy);
		//	}
		//}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		
	}
}
