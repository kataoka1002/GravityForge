#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	
}

Game::~Game()
{
	//削除
	DeleteGO(m_skyCube);
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_camera);
	DeleteGO(m_light);
	DeleteGO(m_teapot);
	DeleteGO(m_reticle);
}

bool Game::Start()
{
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_levelRender.Init("Assets/modelData/level/newLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//名前がhumanだったら。
			if (objData.EqualObjectName(L"player") == true)
			{
				//プレイヤーのオブジェクトを作成する。
				m_player = NewGO<nsPlayer::Player>(0, "player");
				//座標を設定する。
				m_player->SetPosition(objData.position);
				//大きさを設定する。
				m_player->SetScale(objData.scale);
				//回転を設定する。
				m_player->SetRotation(objData.rotation);
				//trueにすると、レベルの方でモデルが読み込まれます。
				return true;
			}
			//名前がbackgroundだったら。
			else if (objData.EqualObjectName(L"background") == true)
			{
				//背景オブジェクトを作成する。
				m_bg = NewGO<BackGround>(0, "background");
				//座標を設定する。
				m_bg->SetPosition(objData.position);
				//大きさを設定する。
				m_bg->SetScale(objData.scale);
				//回転を設定する。
				m_bg->SetRotation(objData.rotation);
				return true;
			}
			//名前がbackgroundだったら。
			else if (objData.EqualObjectName(L"house") == true)
			{
				//背景オブジェクトを作成する。
				m_house = NewGO<House>(0, "house");
				//座標を設定する。
				m_house->SetPosition(objData.position);
				//大きさを設定する。
				m_house->SetScale(objData.scale);
				//回転を設定する。
				m_house->SetRotation(objData.rotation);
				return true;
			}
			//名前がteapotだったら。
			else if (objData.EqualObjectName(L"teapot") == true)
			{
				//ティーポットオブジェクトを作成する。
				m_teapot = NewGO<Teapot>(0, "teapot");
				//座標を設定する。
				m_teapot->SetPosition(objData.position);
				//大きさを設定する。
				m_teapot->SetScale(objData.scale);
				//回転を設定する。
				m_teapot->SetRotation(objData.rotation);
				//リストに追加
				m_objectList.push_back(m_teapot);
				return true;
			}
			//名前がhumanEnemyだったら。
			else if (objData.EqualObjectName(L"humanEnemy") == true)
			{
				//ティーポットオブジェクトを作成する。
				m_humanEnemy = NewGO<nsHumanEnemy::HumanEnemy>(0, "humanenemy");
				//座標を設定する。
				m_humanEnemy->SetPosition(objData.position);
				//大きさを設定する。
				m_humanEnemy->SetScale(objData.scale);
				//回転を設定する。
				m_humanEnemy->SetRotation(objData.rotation);
				//リストに追加
				m_enemyList.push_back(m_humanEnemy);
				return true;
			}
			return true;
		});
	
	//スカイキューブの作成
	SetSkyCube();

	//レティクルの作成
	m_reticle = NewGO<nsPlayer::Reticle>(0, "reticle");

	//ゲームカメラの作成
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//ライトの作成
	m_light = NewGO<Lighting>(0, "lighting");

	return true;
}

void Game::Update()
{
	
}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(1000.0f);
	m_skyCube->SetPosition(m_player->GetPosition());
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}