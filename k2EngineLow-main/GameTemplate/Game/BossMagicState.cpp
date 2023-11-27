#include "stdafx.h"
#include "BossMagicState.h"

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
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossMagicState::Update()
	{

	}
}
