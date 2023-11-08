#include "stdafx.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyDeadState.h"

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
		if (m_enemy->GetHP() <= 50.0f)
		{
			return new HumanEnemyDeadState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyWalkState::Update()
	{
		m_enemy->FollowPlayer();
		m_enemy->Turn();
	}
}
