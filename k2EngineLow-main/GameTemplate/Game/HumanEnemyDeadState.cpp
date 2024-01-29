#include "stdafx.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyTrembleState.h"

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
		// アニメーションの再生が終わったら四つん這い
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//アニメーション速度をもとにもどす
			m_enemy->SetAnimationSpeed(1.0f);

			//キャラコンの再設定を行う
			m_enemy->ResetCharaCon();

			//コリジョンの設定
			//m_enemy->SetCollision();

			//四つん這いをtrueにする
			m_enemy->SetCrawlNow();

			return new HumanEnemyTrembleState(m_enemy);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyDeadState::Update()
	{
		
	}
}
