#include "stdafx.h"
#include "HumanEnemyTrembleState.h"
#include "HumanEnemyDieState.h"
#include "HumanEnemyCrawlState.h"

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
		if (m_enemy->GetHP() <= DEAD_LINE)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		//一定距離以下になると追いかけてくる
		if (m_enemy->CheckDistanceToPlayer() <= CHASE_RANGE_KNEELING)
		{
			return new HumanEnemyCrawlState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyTrembleState::Update()
	{
		//コリジョンの処理
		m_enemy->MoveCollision();
	}
}
