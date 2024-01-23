#include "stdafx.h"
#include "BossIdleState.h"
#include "BossDeadState.h"
#include "BossWalkState.h"
#include "BossMagicState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossIdleState::~BossIdleState()
	{

	}

	void BossIdleState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Idle, 0.3f);
	}

	IBossState* BossIdleState::StateChange()
	{
		if (/*m_boss->CheckDistanceToPlayer() >= FOLLOW_RANGE &&*/ m_boss->GetIsMagicCoolDown() != true && m_boss->GetIsHitCoolDown() != true)
		{
			return new BossWalkState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossIdleState::Update()
	{
		//マジックのクールダウンタイム中なら
		if (m_boss->GetIsMagicCoolDown() == true)
		{
			//クールダウンタイムの計算
			m_boss->CalcMagicCoolDown();
		}

		//ヒットのクールダウンタイム中なら
		if (m_boss->GetIsHitCoolDown() == true)
		{
			//クールダウンタイムの計算
			m_boss->CalcHitCoolDown();
		}
	}
}
