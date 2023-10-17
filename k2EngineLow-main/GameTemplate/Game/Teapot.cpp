#include "stdafx.h"
#include "Teapot.h"

Teapot::Teapot()
{

}

Teapot::~Teapot()
{

}

void Teapot::Update()
{
	ManageState();

	m_charaCon.SetPosition(m_position);
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.Update();
}

void Teapot::InitModel()
{
	m_position = { 0.0f,50.0f,240.0f };

	m_model.Init("Assets/modelData/object/teapot.tkm");
	m_model.SetPosition(m_position);
	m_model.SetScale(0.8f);
	m_model.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		20.0f,			//半径
		10.0f,			//高さ
		m_position		//座標
	);

}

void Teapot::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}