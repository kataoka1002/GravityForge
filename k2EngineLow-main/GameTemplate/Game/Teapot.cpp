#include "stdafx.h"
#include "Teapot.h"
#include "Game.h"

Teapot::Teapot()
{

}

Teapot::~Teapot()
{
	DeleteGO(m_collisionObject);
}

void Teapot::Update()
{
	//動き
	Move();


	m_model.Update();
}

void Teapot::InitModel()
{
	m_model.Init("Assets/modelData/object/teapot.tkm");
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
}

void Teapot::OnDestroy()
{
	//リストから削除
	m_game->RemoveObjectFromList(this);

	//自分自身の削除
	DeleteGO(this);
}

void Teapot::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}