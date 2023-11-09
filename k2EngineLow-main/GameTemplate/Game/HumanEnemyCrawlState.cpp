#include "stdafx.h"
#include "HumanEnemyCrawlState.h"
#include "HumanEnemyDieState.h"
#include "HumanEnemyTrembleState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyCrawlState::~HumanEnemyCrawlState()
	{

	}

	void HumanEnemyCrawlState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Crawl, 0.4f);
	}

	IHumanEnemyState* HumanEnemyCrawlState::StateChange()
	{
		// 体力が0以下で死亡
		if (m_enemy->GetHP() <= DEAD_LINE)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		//一定距離以上になると追いかけてこなくなる
		if (m_enemy->CheckDistanceToPlayer() >= CHASE_RANGE_KNEELING)
		{
			return new HumanEnemyTrembleState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{
		//プレイヤーを追いかける
		m_enemy->FollowPlayer();

		//回転処理
		m_enemy->Turn();

		//コリジョンの処理
		m_enemy->MoveCollision();
	}
}
