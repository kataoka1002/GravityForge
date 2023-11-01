#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerIdleState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerJumpState::~PlayerJumpState()
	{

	}

	void PlayerJumpState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Jump, 0.1f);

		//その場ジャンプ中にする
		m_player->SetIsJumping(true);
	}

	IPlayerState* PlayerJumpState::StateChange()
	{
		//アニメーションの再生が終わったら。
		if (m_player->GetOnGround() == true)
		{
			return new PlayerIdleState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerJumpState::Update()
	{
		// 移動処理。
		m_player->Move();
	}
}