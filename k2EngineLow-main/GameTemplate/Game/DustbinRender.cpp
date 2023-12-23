#include "stdafx.h"
#include "DustbinRender.h"

bool DustbinRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/dustbin.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void DustbinRender::Update()
{

}

void DustbinRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}