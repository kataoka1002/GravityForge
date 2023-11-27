#include "stdafx.h"
#include "BossWalkState.h"
#include "BossSwipeState.h"

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
			return new BossSwipeState(m_boss);
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
