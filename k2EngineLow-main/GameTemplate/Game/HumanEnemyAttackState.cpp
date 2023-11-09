#include "stdafx.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyIdleState.h"

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

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		
	}
}
