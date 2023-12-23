#include "stdafx.h"
#include "ConeRender.h"

bool ConeRender::Start()
{
	//インスタンシング描画用モデルの初期化
	m_modelRender.InitInstancing(
		"Assets/modelData/object/cone.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void ConeRender::Update()
{

}

void ConeRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}