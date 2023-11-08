#include "stdafx.h"
#include "PlayerDeadState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerDeadState::~PlayerDeadState()
	{

	}

	void PlayerDeadState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Dead, 0.5f);
	}

	IPlayerState* PlayerDeadState::StateChange()
	{
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerDeadState::Update()
	{
		
	}
}