#include "stdafx.h"
#include "PlantLongRender.h"

bool PlantLongRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/plant_long.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void PlantLongRender::Update()
{

}

void PlantLongRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}