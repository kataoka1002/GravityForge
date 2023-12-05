#include "stdafx.h"
#include "GameInformation.h"

namespace
{
	const int ENABLE_BOSSBATTLE_ENEMY_NUM = 1;
}

GameInformation::~GameInformation()
{

}

bool GameInformation::Start()
{
	return true;
}

void GameInformation::Update()
{
	//ボス戦なら
	if (m_isInBossBattle)
	{
		//ボス撃破時間の計測
		m_bossKillTime += g_gameTime->GetFrameDeltaTime();
	}
}