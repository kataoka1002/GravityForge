#include "stdafx.h"
#include "BossJumpAttackState.h"
#include "BossIdleState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossJumpAttackState::~BossJumpAttackState()
	{

	}

	void BossJumpAttackState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_JumpAttack, 0.3f);

		//目的地の設定
		m_boss->SetJumpTarget();
	}

	IBossState* BossJumpAttackState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossJumpAttackState::Update()
	{
		//ジャンプ攻撃の処理
		m_boss->JumpAttack();
	}
}
