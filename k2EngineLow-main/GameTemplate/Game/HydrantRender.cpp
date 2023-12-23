#include "stdafx.h"
#include "HydrantRender.h"

bool HydrantRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/hydrant.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void HydrantRender::Update()
{

}

void HydrantRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}