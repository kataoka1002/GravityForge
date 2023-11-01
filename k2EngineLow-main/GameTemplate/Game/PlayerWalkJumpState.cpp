#include "stdafx.h"
#include "PlayerWalkJumpState.h"
#include "PlayerWalkState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerWalkJumpState::~PlayerWalkJumpState()
	{

	}

	void PlayerWalkJumpState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_WalkJump, 0.2f);
	}

	IPlayerState* PlayerWalkJumpState::StateChange()
	{
		//アニメーションの再生が終わったら。
		if (m_player->GetOnGround() == true)
		{
			return new PlayerWalkState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerWalkJumpState::Update()
	{
		// 移動処理。
		m_player->Move();

		// 回転処理。
		m_player->Turn();
	}
}