#include "stdafx.h"
#include "HumanEnemyTrembleState.h"

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
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Dead, 0.5f);
	}

	IHumanEnemyState* HumanEnemyTrembleState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyTrembleState::Update()
	{
		
	}
}
