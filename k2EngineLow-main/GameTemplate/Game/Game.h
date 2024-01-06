#pragma once
#include "SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Lighting.h"
#include "Boss.h"

#include "Teapot.h"
#include "Air.h"
#include "BarrierFence.h"
#include "BenchBig.h"
#include "BenchSmall.h"
#include "BillboardSmall.h"
#include "BushBig.h"
#include "Cone.h"
#include "Dustbin.h"
#include "Fence.h"
#include "Hydrant.h"
#include "PlantLong.h"
#include "PlantLow.h"
#include "RockBig.h"
#include "SolarPanel.h"

#include "Reticle.h"
#include "level3D/Level.h"
#include "ObjectBase.h"
#include "HumanEnemy.h"
#include "House.h"
#include "PlayerUI.h"
#include "Wall.h"
#include "GameInformation.h"
#include "BlackFade.h"
#include "MissionUI.h"
#include "MakeEffect.h"
#include "HumanEnemyRender.h"

#include "AirRender.h"
#include "BarrierFenceRender.h"
#include "BenchiBigRender.h"
#include "BenchiSmallRender.h"
#include "BillboardSmallRender.h"
#include "BushBigRender.h"
#include "ConeRender.h"
#include "DustbinRender.h"
#include "FenceRender.h"
#include "HydrantRender.h"
#include "PlantLongRender.h"
#include "PlantLowRender.h"
#include "RockBigRender.h"
#include "SolarPanelRender.h"


class Game : public IGameObject, Noncopyable
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetSkyCube();
	template <typename T>
	void SetLevel(T* objct, LevelObjectData& objData);
	void SetGameClear();
	void SetGameOver();

	/// <summary>
	/// オブジェクトをリストから削除する
	/// </summary>
	/// <param name="teapot"></param>
	void RemoveObjectFromList(ObjectBase* object)
	{
		std::vector<ObjectBase*>::iterator it = std::find(
			m_objectList.begin(), 	//オブジェクトのリストの最初
			m_objectList.end(), 	//オブジェクトのリストの最後
			object);				//探しているオブジェクト

		if (it != m_objectList.end())
		{
			m_objectList.erase(it);
		}
	}

	/// <summary>
	/// エネミーをリストから削除する
	/// </summary>
	/// <param name="teapot"></param>
	void RemoveEnemyFromList(EnemyBase* enemy)
	{
		std::vector<EnemyBase*>::iterator it = std::find(
			m_enemyList.begin(), 	//オブジェクトのリストの最初
			m_enemyList.end(), 		//オブジェクトのリストの最後
			enemy);					//探しているオブジェクト

		if (it != m_enemyList.end())
		{
			m_enemyList.erase(it);
		}
	}

	/// <summary>
	/// オブジェクトのリストを返す
	/// </summary>
	/// <returns></returns>
	const std::vector<ObjectBase*>& GetObjectList() const
	{
		return m_objectList;
	}

	/// <summary>
	/// エネミーのリストを返す
	/// </summary>
	/// <returns></returns>
	const std::vector<EnemyBase*>& GetEnemyList() const
	{
		return m_enemyList;
	}


private:
	Level m_levelRender;
	SkyCube* m_skyCube = nullptr;
	nsPlayer::Player* m_player = nullptr;
	nsPlayer::Reticle* m_reticle = nullptr;
	nsPlayer::PlayerUI* m_playerUI = nullptr;
	BackGround* m_bg = nullptr;
	GameCamera* m_camera = nullptr;
	Lighting* m_light = nullptr;
	nsBoss::Boss* m_boss = nullptr;

	Teapot* m_teapot = nullptr;
	Air* m_air = nullptr;
	BarrierFence* m_barrierFence = nullptr;
	BenchBig* m_benchBig = nullptr;
	BenchSmall* m_benchSmall = nullptr;
	BillboardSmall* m_billboardSmall = nullptr;
	BushBig* m_bushBig = nullptr;
	Cone* m_cone = nullptr;
	Dustbin* m_dustbin = nullptr;
	Fence* m_fence = nullptr;
	Hydrant* m_hydrant = nullptr;
	PlantLong* m_plantLong = nullptr;
	PlantLow* m_plantLow = nullptr;
	RockBig* m_rockBig = nullptr;
	SolarPanel* m_solarPanel = nullptr;

	nsHumanEnemy::HumanEnemy* m_humanEnemy = nullptr;
	nsHumanEnemy::HumanEnemyRender* m_humanEnemyRender = nullptr;
	House* m_house = nullptr;
	Wall* m_wall = nullptr;
	GameInformation* m_gameInfo = nullptr;
	BlackFade* m_blackFade = nullptr;
	MissionUI* m_missionUI = nullptr;
	MakeEffect* m_makeEfe = nullptr;

	//インスタンシング描画用のレンダラー
	AirRender* m_airRender = nullptr;
	BarrierFenceRender* m_barrierFenceRender = nullptr;
	BenchiBigRender* m_benchBigRender = nullptr;
	BenchiSmallRender* m_benchSmallRender = nullptr;
	BillboardSmallRender* m_billboardSmallRender = nullptr;
	BushBigRender* m_bushBigRender = nullptr;
	ConeRender* m_coneRender = nullptr;
	DustbinRender* m_dustbinRender = nullptr;
	FenceRender* m_fenceRender = nullptr;
	HydrantRender* m_hydrantRender = nullptr;
	PlantLongRender* m_plantLongRender = nullptr;
	PlantLowRender* m_plantLowRender = nullptr;
	RockBigRender* m_rockBigRender = nullptr;
	SolarPanelRender* m_solarPanelRender = nullptr;

	std::vector<EnemyBase*> m_enemyList;			//エネミーのリスト
	std::vector<ObjectBase*> m_objectList;			//ティーポットのリスト

	bool m_gameClear = false;
	bool m_gameOver = false;
};

