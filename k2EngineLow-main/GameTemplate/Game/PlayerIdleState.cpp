#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerAttractState.h"
#include "PlayerWalkState.h"

namespace
{

}

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerIdleState::~PlayerIdleState()
	{

	}

	void PlayerIdleState::Enter()
	{
		// 再生するアニメーションを設定。
		m_player->SetAnimation(Player::enAnimClip_Idle, 0.5f);
	}

	IPlayerState* PlayerIdleState::StateChange()
	{
		if (m_player->GetCanAttract()) 
		{
			// 引き寄せ不可の状態に戻す
			m_player->SetCanAttract(false);

			// 引き寄せステートに遷移する。
			return new PlayerAttractState(m_player);
		}

		//スティックの入力量によってステートを変更
		if(g_pad[0]->GetLStickXF() != 0.000f || g_pad[0]->GetLStickYF() != 0.000f)
		{
			// 歩きステートに遷移する
			return new PlayerWalkState(m_player);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void PlayerIdleState::Update()
	{		
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//引き寄せる処理
			m_player->Attract();
		}
	}
}