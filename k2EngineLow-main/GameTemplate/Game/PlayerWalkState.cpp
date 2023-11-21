#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerAttractState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkJumpState.h"
#include "PlayerPunchState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerWalkState::~PlayerWalkState()
	{

	}

	void PlayerWalkState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Walk, 0.2f);
	}

	IPlayerState* PlayerWalkState::StateChange()
	{
		if (m_player->GetCanAttract())
		{
			// 引き寄せ不可の状態に戻す
			m_player->SetCanAttract(false);

			// 引き寄せステートに遷移する。
			return new PlayerAttractState(m_player);
		}

		if (g_pad[0]->GetLStickXF() == 0.000f || g_pad[0]->GetLStickYF() == 0.000f)
		{
			// アイドルステートに遷移する
			return new PlayerIdleState(m_player);
		}

		if (g_pad[0]->IsTrigger(enButtonX))
		{
			//ジャンプする
			m_player->SetJump();

			// 歩きジャンプステートに遷移する
			return new PlayerWalkJumpState(m_player);
		}

		if (g_pad[0]->IsTrigger(enButtonY))
		{
			// パンチステートに遷移する
			return new PlayerPunchState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerWalkState::Update()
	{
		// 移動処理。
		m_player->Move();

		// 回転処理。
		m_player->Turn();
	}
}