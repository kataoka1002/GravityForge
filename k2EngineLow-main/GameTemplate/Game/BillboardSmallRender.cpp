#include "stdafx.h"
#include "BillboardSmallRender.h"

bool BillboardSmallRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/billboard_small.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BillboardSmallRender::Update()
{

}

void BillboardSmallRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}