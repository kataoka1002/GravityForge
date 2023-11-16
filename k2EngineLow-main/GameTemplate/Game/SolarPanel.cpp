#include "stdafx.h"
#include "SolarPanel.h"
#include "Game.h"

namespace
{
	//コリジョンの大きさ
	const float COLLISION_SCALE = 30.0f;

	//与えるダメージ
	const float DAMAGE_AMOUNT = 50.0f;
}

SolarPanel::SolarPanel()
{

}

SolarPanel::~SolarPanel()
{
	DeleteGO(m_collisionObject);
}

void SolarPanel::Update()
{
	//動き
	Move();


	m_model.Update();
}

void SolarPanel::InitModel()
{
	m_model.Init("Assets/modelData/object/solarPanel.tkm");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		20.0f,			//半径
		10.0f,			//高さ
		m_position		//座標
	);

	//与えるダメージの設定
	m_damage = DAMAGE_AMOUNT;
}

void SolarPanel::InitCollision()
{
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(
		m_position,				//座標
		Quaternion::Identity,	//回転
		COLLISION_SCALE			//半径
	);

	//コリジョンに名前を付ける
	m_collisionObject->SetName("solarPanel");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void SolarPanel::OnDestroy()
{
	//リストから削除
	m_game->RemoveObjectFromList(this);

	//自分自身の削除
	DeleteGO(this);
}

void SolarPanel::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}