#include "stdafx.h"
#include "BossSwipeState.h"
#include "BossIdleState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossSwipeState::~BossSwipeState()
	{

	}

	void BossSwipeState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Swipe, 0.3f);
	}

	IBossState* BossSwipeState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossSwipeState::Update()
	{

	}
}
