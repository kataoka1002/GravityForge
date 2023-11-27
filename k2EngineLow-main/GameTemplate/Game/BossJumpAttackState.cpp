#include "stdafx.h"
#include "BossJumpAttackState.h"

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
	}

	IBossState* BossJumpAttackState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossJumpAttackState::Update()
	{

	}
}
