#include "stdafx.h"
#include "BossPunchState.h"
#include "BossIdleState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossPunchState::~BossPunchState()
	{

	}

	void BossPunchState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Punch, 0.3f);
	}

	IBossState* BossPunchState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossPunchState::Update()
	{

	}
}
