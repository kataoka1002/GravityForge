#include "stdafx.h"
#include "SolarPanelRender.h"

bool SolarPanelRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/solarPanel.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void SolarPanelRender::Update()
{

}

void SolarPanelRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}