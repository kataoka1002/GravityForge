#include "stdafx.h"
#include "RockBig.h"
#include "Game.h"
#include "RockBigRender.h"

namespace
{
	//コリジョンの大きさ
	const float COLLISION_SCALE = 80.0f;

	//与えるダメージ
	const float DAMAGE_AMOUNT = 50.0f;

	//キャラコンの半径
	const float CHARACON_RADIUS = 40.0f;

	//キャラコンの高さ
	const float CHARACON_HEIGHT = 10.0f;
}

RockBig::RockBig()
{

}

RockBig::~RockBig()
{
	DeleteGO(m_collisionObject);
	// 先に死んでいるかもしれないので、検索してnullチェックをする。
	auto render = FindGO<RockBigRender>("rockbigrender");
	if (render) {
		render->RemoveInstance(m_instanceNo);
	}
}

void RockBig::Update()
{
	//動き
	Move();

	//m_model.Update();

	//モデルの更新処理
	m_rockBigRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void RockBig::InitModel()
{
	m_rockBigRender = FindGO<RockBigRender>("rockbigrender");

	//m_model.Init("Assets/modelData/object/rock_big.tkm");
	//m_model.SetPosition(m_position);
	//m_model.SetRotation(m_rotation);
	//m_model.SetScale(m_scale);
	//m_model.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		CHARACON_RADIUS,	//半径
		CHARACON_HEIGHT,	//高さ
		m_position			//座標
	);

	//与えるダメージの設定
	m_damage = DAMAGE_AMOUNT;

	//コリジョンを出す座標の加算量を求める
	if (CHARACON_RADIUS >= CHARACON_HEIGHT)
	{
		m_collisionAddPos = CHARACON_RADIUS;
	}
	else
	{
		m_collisionAddPos = CHARACON_HEIGHT;
	}
}

void RockBig::InitCollision()
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
	m_collisionObject->SetName("rock_big");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void RockBig::OnDestroy()
{
	//リストから削除
	m_game->RemoveObjectFromList(this);

	//自分自身の削除
	DeleteGO(this);
}

void RockBig::Render(RenderContext& rc)
{
	//モデルが見えない位置にあるなら描画しない
	if (m_canSeeModel != true)
	{
		return;
	}

	//m_model.Draw(rc);
}