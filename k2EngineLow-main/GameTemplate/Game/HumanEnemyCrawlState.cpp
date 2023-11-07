#include "stdafx.h"
#include "HumanEnemyCrawlState.h"

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
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{

	}
}
