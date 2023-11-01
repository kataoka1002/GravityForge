#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"

bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");

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