#include "stdafx.h"
#include "BarrierFenceRender.h"

bool BarrierFenceRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/barrierFence.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BarrierFenceRender::Update()
{

}

void BarrierFenceRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}