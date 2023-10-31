#include "stdafx.h"
#include "PlayerAttractState.h"
#include "PlayerStandbyState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerAttractState::~PlayerAttractState()
	{

	}

	void PlayerAttractState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Attract, 0.5f);

		// オブジェクト保持状態にする
		m_player->SetIsHoldingObject(true);
	}

	IPlayerState* PlayerAttractState::StateChange()
	{
		//アニメーションの再生が終わったら。
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//スタンバイにする
			return new PlayerStandbyState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerAttractState::Update()
	{
		// 回転処理。
		m_player->Turn();
	}
}