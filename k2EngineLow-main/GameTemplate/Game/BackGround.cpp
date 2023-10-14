#include "stdafx.h"
#include "BackGround.h"

namespace
{
	const Vector3 BG_POSITION = { 0.0f,0.0f,-1000.0f };
}

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	//背景モデルの初期化
	m_bgModel.Init("Assets/modelData/stage/background.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bgModel.SetPosition(BG_POSITION);
	m_bgModel.Update();

	// 静的物理オブジェクトを作成。
	m_phyStaticObject.CreateFromModel(m_bgModel.GetModel(), m_bgModel.GetModel().GetWorldMatrix());

	return true;
}

void BackGround::Update()
{

}

void BackGround::Render(RenderContext& rc)
{
	m_bgModel.Draw(rc);
}