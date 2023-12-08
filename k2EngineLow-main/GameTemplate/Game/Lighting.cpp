#include "stdafx.h"
#include "Lighting.h"

namespace
{
	const Vector3 DIRECTION_LIG_COLOR = { 2.9f,2.9f,2.9f };
}

bool Lighting::Start()
{
	//ディレクションライトの方向の設定
	Vector3 dir = { 1.0f,-1.0f,0.0f };
	dir.Normalize();

	//ディレクションライトの設定
	g_renderingEngine->SetDirectionLight(0, dir, DIRECTION_LIG_COLOR);

	return true;
}

void Lighting::Update()
{

}