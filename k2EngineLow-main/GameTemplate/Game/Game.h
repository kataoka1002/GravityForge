#pragma once
#include "SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Lighting.h"
#include "Teapot.h"
#include "Reticle.h"
#include "level3D/Level.h"
#include "ObjectBase.h"
#include "HumanEnemy.h"
#include "House.h"

class Game : public IGameObject, Noncopyable
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetSkyCube();

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
	BackGround* m_bg = nullptr;
	GameCamera* m_camera = nullptr;
	Lighting* m_light = nullptr;
	Teapot* m_teapot = nullptr;
	nsHumanEnemy::HumanEnemy* m_humanEnemy = nullptr;
	House* m_house = nullptr;

	std::vector<EnemyBase*> m_enemyList;			//エネミーのリスト
	std::vector<ObjectBase*> m_objectList;			//ティーポットのリスト
};

