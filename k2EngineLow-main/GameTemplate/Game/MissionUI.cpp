#include "stdafx.h"
#include "MissionUI.h"
#include "GameInformation.h"

MissionUI::~MissionUI()
{

}

bool MissionUI::Start()
{
	//インフォメーションのインスタンスを探す。
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//画像の初期化
	m_missionSprite.Init("Assets/spriteData/senmetu.dds", 264.0f, 66.0f);
	m_missionSprite.SetPosition({ -600.0f,350.0f,0.0f });
	m_missionSprite.SetScale(0.8f);
	m_missionSprite.Update();

	m_missionZombie.Init("Assets/spriteData/zonbiHaijo.dds", 319.0f, 52.0f);
	m_missionZombie.SetPosition({ -660.0f,310.0f,0.0f });
	m_missionZombie.SetScale(0.8f);
	m_missionZombie.SetPivot({ 0.0f, 0.5f });
	m_missionZombie.Update();

	m_missionBoss.Init("Assets/spriteData/doomToubatu.dds", 281.0f, 51.0f);
	m_missionBoss.SetPosition({ -660.0f,310.0f,0.0f });
	m_missionBoss.SetScale(0.8f);
	m_missionBoss.SetPivot({ 0.0f, 0.5f });
	m_missionBoss.Update();

	return true;
}

void MissionUI::Update()
{

}

void MissionUI::Render(RenderContext& rc)
{
	if (m_gameInfo->GetIsInGame())
	{
		m_missionSprite.Draw(rc);
		m_missionZombie.Draw(rc);
	}

	if (m_gameInfo->GetIsInBossBattle())
	{
		m_missionBoss.Draw(rc);
	}
}