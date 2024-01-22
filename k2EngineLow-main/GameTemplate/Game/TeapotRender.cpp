#include "stdafx.h"
#include "TeapotRender.h"

bool TeapotRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/policeCar.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void TeapotRender::Update()
{

}

void TeapotRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}