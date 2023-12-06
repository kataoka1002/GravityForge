#include "stdafx.h"
#include "PlayerReactionState.h"
#include "PlayerIdleState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerReactionState::~PlayerReactionState()
	{

	}

	void PlayerReactionState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Reaction, 0.1f);
	}

	IPlayerState* PlayerReactionState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//攻撃中ではなくする
			m_player->SetCanAttack(false);

			//プレイヤーの情報をリセット
			m_player->ResetAttack();

			return new PlayerIdleState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerReactionState::Update()
	{
		
	}
}