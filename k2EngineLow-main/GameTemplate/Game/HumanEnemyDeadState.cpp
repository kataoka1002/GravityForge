#include "stdafx.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyCrawlState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyDeadState::~HumanEnemyDeadState()
	{

	}

	void HumanEnemyDeadState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Dead, 0.5f);
	}

	IHumanEnemyState* HumanEnemyDeadState::StateChange()
	{
		// アニメーションの再生が終わったら
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//キャラコンの再設定を行う
			m_enemy->ResetCharaCon();

			return new HumanEnemyCrawlState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyDeadState::Update()
	{
		
	}
}
