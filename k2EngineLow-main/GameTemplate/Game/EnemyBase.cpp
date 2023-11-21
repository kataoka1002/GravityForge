#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"

bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");

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

			return true;
		}
	}

	return false;
}