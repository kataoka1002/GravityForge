#include "stdafx.h"
#include "BenchiBigRender.h"

bool BenchiBigRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/bench_big.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BenchiBigRender::Update()
{

}

void BenchiBigRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}