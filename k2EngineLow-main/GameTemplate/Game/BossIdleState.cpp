#include "stdafx.h"
#include "BossIdleState.h"
#include "BossDeadState.h"

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
		//攻撃を受けたら
		if (m_boss->DidAttackHit())
		{
			// 体力が0以下で死亡
			if (m_boss->GetHP() <= 0.0f)
			{
				return new BossDeadState(m_boss);
			}
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossIdleState::Update()
	{

	}
}
