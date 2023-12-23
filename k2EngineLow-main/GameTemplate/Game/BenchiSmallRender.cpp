#include "stdafx.h"
#include "BenchiSmallRender.h"

bool BenchiSmallRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/bench_small.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BenchiSmallRender::Update()
{

}

void BenchiSmallRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}