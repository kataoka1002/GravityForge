#include "stdafx.h"
#include "AirRender.h"

bool AirRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/air.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxAir
	);
	return true;
}

void AirRender::Update()
{

}

void AirRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}