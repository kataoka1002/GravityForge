#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//‰“•½–Ê‚Ìİ’è
	g_camera3D->SetFar(15000.0f);

	return true;
}

void GameCamera::Update()
{
	auto pos = g_camera3D->GetPosition();
	auto target = g_camera3D->GetTarget();
	pos.z -= g_pad[0]->GetLStickYF() * 2.0f;
	target.z -= g_pad[0]->GetLStickYF() * 2.0f;
	pos.y += g_pad[0]->GetRStickYF() * 2.0f;
	target.y += g_pad[0]->GetRStickYF() * 2.0f;
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(target);
}