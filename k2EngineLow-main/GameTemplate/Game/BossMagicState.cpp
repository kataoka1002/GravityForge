#include "stdafx.h"
#include "BossMagicState.h"
#include "BossIdleState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossMagicState::~BossMagicState()
	{

	}

	void BossMagicState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Magic, 0.3f);
	}
	
	IBossState* BossMagicState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//マジックのクールダウンタイム中にする
			m_boss->SetMagicCoolDown();

			return new BossIdleState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossMagicState::Update()
	{

	}
}
