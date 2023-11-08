#include "stdafx.h"
#include "HumanEnemyCrawlState.h"
#include "HumanEnemyDieState.h"

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
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Crawl, 0.5f);
	}

	IHumanEnemyState* HumanEnemyCrawlState::StateChange()
	{
		// 体力が0以下で死亡
		if (m_enemy->GetHP() <= 0.0f)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{
		m_enemy->FollowPlayer();
		m_enemy->Turn();
		m_enemy->MoveCollision();
	}
}
