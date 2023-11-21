#include "stdafx.h"
#include "BossDeadState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossDeadState::~BossDeadState()
	{

	}

	void BossDeadState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Dead, 0.3f);
	}

	IBossState* BossDeadState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossDeadState::Update()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//ゲームクリア画面へ
			m_boss->GameClear();
		}
	}
}
