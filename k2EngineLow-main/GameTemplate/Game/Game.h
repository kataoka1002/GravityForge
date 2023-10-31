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
	void RemoveTeapotFromList(ObjectBase* object)
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

	const std::vector<ObjectBase*>& GetObjectList() const
	{
		return m_objectList;
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
	HumanEnemy* m_humanEnemy = nullptr;

	//std::vector<Teapot*> m_teapotList;			//ティーポットのリスト
	std::vector<ObjectBase*> m_objectList;			//ティーポットのリスト
};

