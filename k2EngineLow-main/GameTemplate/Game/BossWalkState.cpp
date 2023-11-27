#include "stdafx.h"
#include "BossWalkState.h"
#include "BossSwipeState.h"
#include "BossPunchState.h"
#include "BossMagicState.h"
#include "BossJumpAttackState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossWalkState::~BossWalkState()
	{

	}

	void BossWalkState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Walk, 0.3f);
	}

	IBossState* BossWalkState::StateChange()
	{
		if (m_boss->CheckDistanceToPlayer() < FOLLOW_RANGE)
		{
			//ランダムな値を取る
			int num = std::rand() % 2 + 1;

			if (num == 1)
			{
				//切り裂き攻撃
				return new BossSwipeState(m_boss);
			}
			if (num == 2)
			{
				//パンチ
				return new BossPunchState(m_boss);
			}
		}

		if (std::rand() % 100 + 1 == 1)
		{
			return new BossJumpAttackState(m_boss);
		}

		if (std::rand() % 500 + 1 == 1)
		{
			return new BossMagicState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossWalkState::Update()
	{
		//プレイヤーを追いかける
		m_boss->FollowPlayer();

		//回転する
		m_boss->Turn();
	}
}
