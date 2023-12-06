#include "stdafx.h"
#include "PlayerStandbyState.h"
#include "PlayerAttackState.h"

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerStandbyState::~PlayerStandbyState()
	{

	}

	void PlayerStandbyState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Standby, 0.5f);
	}

	IPlayerState* PlayerStandbyState::StateChange()
	{
		if (m_player->GetCanAttack())
		{
			//攻撃中ではなくする
			m_player->SetCanAttack(false);

			//攻撃ステートに遷移
			return new PlayerAttackState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerStandbyState::Update()
	{
		// 移動処理。
		m_player->Move();

		// 回転処理。
		m_player->Turn();

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			// 攻撃処理
			m_player->Attack();
		}

		// 歩き方の変更処理
		m_player->ChangeWalkingStyle();
	}
}