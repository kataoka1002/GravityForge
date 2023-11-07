#include "stdafx.h"
#include "HumanEnemyTrembleState.h"
#include "HumanEnemyDieState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyTrembleState::~HumanEnemyTrembleState()
	{

	}

	void HumanEnemyTrembleState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Tremble, 0.5f);
	}

	IHumanEnemyState* HumanEnemyTrembleState::StateChange()
	{
		// 体力が0以下で死亡
		if (m_enemy->GetHP() <= 0.0f)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyTrembleState::Update()
	{
		
	}
}
