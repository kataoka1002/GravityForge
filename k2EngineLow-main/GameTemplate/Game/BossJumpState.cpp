#include "stdafx.h"
#include "BossJumpState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossJumpState::~BossJumpState()
	{

	}

	void BossJumpState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Jump, 0.3f);
	}

	IBossState* BossJumpState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossJumpState::Update()
	{

	}
}
