#include "stdafx.h"
#include "Test.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

Test::Test()
{
	animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);

	m_model.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimClip_Num, enModelUpAxisY);
	//m_bg.Init("Assets/modelData/bg.tkm", nullptr, 0, enModelUpAxisZ, true);
	//m_model.Init("Assets/modelData/bg/testModel.tkm");
	m_bg.Init("Assets/modelData/bg/bg.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bg.SetScale(0.1f);
	m_bg.Update();

	g_renderingEngine->SetDirectionLight(0, Vector3{ 0.0f,0.0f,-1.0f }, Vector3{ 1.0f,1.0f,1.0f });

	g_camera3D->SetFar(15000.0f);
}

Test::~Test()
{

}

bool Test::Start()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(1000.0f);
	m_skyCube->SetPosition({ 0.0f,0.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	return true;
}

void Test::Update()
{
	auto pos = g_camera3D->GetPosition();
	auto target = g_camera3D->GetTarget();
	pos.z -= g_pad[0]->GetLStickYF() * 2.0f;
	target.z -= g_pad[0]->GetLStickYF() * 2.0f;
	pos.y += g_pad[0]->GetRStickYF() * 2.0f;
	target.y += g_pad[0]->GetRStickYF() * 2.0f;
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(target);


	m_model.Update();
}

void Test::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_bg.Draw(rc);
}
