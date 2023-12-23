#include "stdafx.h"
#include "RockBigRender.h"

bool RockBigRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/rock_big.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void RockBigRender::Update()
{

}

void RockBigRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}