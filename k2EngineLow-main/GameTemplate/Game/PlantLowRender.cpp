#include "stdafx.h"
#include "PlantLowRender.h"

bool PlantLowRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/plant_low.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void PlantLowRender::Update()
{

}

void PlantLowRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}