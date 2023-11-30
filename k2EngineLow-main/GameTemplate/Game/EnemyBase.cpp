#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"
#include "GameInformation.h"


bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//モデルの初期化
	InitModel();

	return true;
}

void EnemyBase::HandleDamageEvent(float damage)
{
	//ダメージを与える
	m_hp -= damage;
	m_hp = max(0.0f, m_hp);
}

bool EnemyBase::DidAttackHit()
{
	//クールダウンの計算
	CalcCoolDown();

	//クールダウン中なら返す
	if (m_isCooldown == true)
	{
		return false;
	}

	//攻撃用のコリジョンの配列を取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_punch");
	//配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//ダメージを与える
			HandleDamageEvent(10.0f);

			//リアクションの再生
			PlayReaction();

			//クールダウン中にする
			m_isCooldown = true;

			return true;
		}
	}

	return false;
}

void EnemyBase::CalcCoolDown()
{
	//クールダウン中じゃないなら
	if (m_isCooldown == false)
	{
		return;
	}

	m_cooldownTime -= g_gameTime->GetFrameDeltaTime();

	if (m_cooldownTime <= 0.0f)
	{
		//クールダウン終わり
		m_isCooldown = false;

		//タイマーのリセット
		m_cooldownTime = 1.0f;
	}
}