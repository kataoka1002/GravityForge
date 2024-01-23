#include "stdafx.h"
#include "Teapot.h"
#include "Game.h"
#include "TeapotRender.h"
namespace
{
	//コリジョンの大きさ
	const float COLLISION_RADIUS = 80.0f;

	//コリジョンの高さ
	const float COLLISION_HEIGHT = 200.0f;

	//爆発コリジョンの大きさ
	const float BOMB_COLLISION_SCALE = 300.0f;

	//与えるダメージ
	const float DAMAGE_AMOUNT = 100.0f;

	//キャラコンの半径
	const float CHARACON_RADIUS = 10.0f;

	//キャラコンの高さ
	const float CHARACON_HEIGHT = 10.0f;
}

Teapot::Teapot()
{

}

Teapot::~Teapot()
{
	DeleteGO(m_collisionObject);
	DeleteGO(m_bombCollisionObject);
	DeleteGO(m_carCollision);
	// 先に死んでいるかもしれないので、検索してnullチェックをする。
	auto render = FindGO<TeapotRender>("teapotrender");
	if (render) {
		render->RemoveInstance(m_instanceNo);
	}
}

void Teapot::Update()
{
	//動き
	Move();

	//持ち上げられたらコリジョンを無効化する
	if (m_objectState != enObjectState_Quiescence)
	{
		m_carCollision->SetIsEnable(false);
	}

	//m_model.Update();

	//モデルの更新処理
	m_teapotRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void Teapot::InitModel()
{
	m_teapotRender = FindGO<TeapotRender>("teapotrender");

	/*m_model.Init("Assets/modelData/object/teapot.tkm");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();*/

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		CHARACON_RADIUS,	//半径
		CHARACON_HEIGHT,	//高さ
		m_position			//座標
	);

	//車のコリジョンの初期化
	InitCarCollision();

	//与えるダメージの設定
	m_damage = DAMAGE_AMOUNT;

	m_localPosition = { 150.0f,200.0f,0.0f };
}

void Teapot::InitCarCollision()
{
	//コリジョンオブジェクトを作成する。
	m_carCollision = NewGO<CollisionObject>(0);

	//コリジョンを横に倒す
	Quaternion rot = m_rotation;
	rot.AddRotationDegX(180.0f);

	//カプセル状のコリジョンを作成する。
	m_carCollision->CreateCapsule(
		m_position,				//座標
		rot,					//回転
		COLLISION_RADIUS,		//半径
		COLLISION_HEIGHT		//高さ
	);

	//コリジョンに名前を付ける
	m_carCollision->SetName("carcollision");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_carCollision->SetIsEnableAutoDelete(false);
}

void Teapot::InitCollision()
{
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	//コリジョンを横に倒す
	Quaternion rot = m_rotation;
	rot.AddRotationDegX(180.0f);

	//カプセル状のコリジョンを作成する。
	m_collisionObject->CreateCapsule(
		m_position,				//座標
		rot,					//回転
		COLLISION_RADIUS,		//半径
		COLLISION_HEIGHT		//高さ
	);

	//コリジョンに名前を付ける
	m_collisionObject->SetName("teapot");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Teapot::OnDestroy()
{
	//コリジョンオブジェクトを作成する。
	m_bombCollisionObject = NewGO<CollisionObject>(0);

	//球状のコリジョンを作成する。
	m_bombCollisionObject->CreateSphere(
		m_position,				//座標
		Quaternion::Identity,	//回転
		BOMB_COLLISION_SCALE	//半径
	);

	//コリジョンに名前を付ける
	m_bombCollisionObject->SetName("bomb");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_bombCollisionObject->SetIsEnableAutoDelete(false);

	//爆発の当たり判定の処理
	BombProcess();

	//エフェクトの発生
	PlayEffect(enEffectName_BombLightning, m_position, Quaternion::Identity, Vector3::One);
	PlayEffect(enEffectName_BombLightning, m_position, Quaternion::Identity, Vector3::One);

	//リストから削除
	m_game->RemoveObjectFromList(this);

	//自分自身の削除
	DeleteGO(this);
}

void Teapot::Render(RenderContext& rc)
{
	//モデルが見えない位置にあるなら描画しない
	if (m_canSeeModel != true)
	{
		return;
	}

	//m_model.Draw(rc);
}