#include "stdafx.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerAttackState::~PlayerAttackState()
	{

	}

	void PlayerAttackState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Attack, 0.2f);
	}

	IPlayerState* PlayerAttackState::StateChange()
	{
		//アニメーションの再生が終わったら。
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//プレイヤーの情報をリセット
			m_player->ResetAttack();

			//スタンバイにする
			return new PlayerIdleState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerAttackState::Update()
	{

	}
}