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
		m_boss->SetAnimation(Boss::enAnimClip_Jump, 0.1f);
	}

	IBossState* BossJumpState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossJumpState::Update()
	{
		//重力の処理
		m_boss->Gravity(2.0f);

		//アニメーションが終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//ネームプレートの出現
			m_boss->SetPlate();
		}
	}
}
