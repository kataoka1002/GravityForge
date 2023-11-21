#include "stdafx.h"
#include "HumanEnemyReactionState.h"
#include "HumanEnemyIdleState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyReactionState::~HumanEnemyReactionState()
	{

	}

	void HumanEnemyReactionState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Reaction, 0.3f);
	}

	IHumanEnemyState* HumanEnemyReactionState::StateChange()
	{
		// アニメーションの再生が終わったら
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyReactionState::Update()
	{
		
	}
}
