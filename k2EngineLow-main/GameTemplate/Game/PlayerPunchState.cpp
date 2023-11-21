#include "stdafx.h"
#include "PlayerPunchState.h"
#include "PlayerIdleState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerPunchState::~PlayerPunchState()
	{

	}

	void PlayerPunchState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Punch, 0.2f);
	}

	IPlayerState* PlayerPunchState::StateChange()
	{
		//アニメーションの再生が終わったら。
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//アイドルステートに遷移
			return new PlayerIdleState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerPunchState::Update()
	{

	}
}