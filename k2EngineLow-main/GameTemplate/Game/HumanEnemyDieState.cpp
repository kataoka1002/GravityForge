#include "stdafx.h"
#include "HumanEnemyDieState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyDieState::~HumanEnemyDieState()
	{

	}

	void HumanEnemyDieState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Die, 0.5f);
	}

	IHumanEnemyState* HumanEnemyDieState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyDieState::Update()
	{
		// アニメーションの再生が終わったら
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			// 削除
			m_enemy->OnDestroy();
		}
	}
}
