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
	if (m_defeatEnemyNum >= ENABLE_BOSSBATTLE_ENEMY_NUM)
	{
		m_canGoBossBattle = true;
	}
}