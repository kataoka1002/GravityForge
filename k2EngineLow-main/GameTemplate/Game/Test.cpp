#include "stdafx.h"
#include "Test.h"

Test::Test()
{
	animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);

	m_model.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimClip_Num, enModelUpAxisY);
	m_bg.Init("Assets/modelData/bg.tkm");

	g_renderingEngine->SetHemLight({ 0.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });

	g_renderingEngine->SetDirectionLight(0, Vector3{ 0.0f,0.0f,-1.0f }, Vector3{ 0.5f,0.5f,0.5f });

	g_renderingEngine->SetPointLight(0, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f,0.0f,0.0f }, 200.0f);

	Vector3 dir = { 1.0f,-1.0f,1.0f };
	dir.Normalize();
	g_renderingEngine->SetSpotLight(0, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 10.0f,10.0f,10.0f }, 300.0f, dir, Math::DegToRad(25.0f));
}

Test::~Test()
{

}

void Test::Update()
{
	m_model.PlayAnimation(enAnimClip_Idle);
	m_model.Update();

	if (g_pad[0]->IsPress(enButtonRight))
	{
		pos.x += 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonLeft))
	{
		pos.x -= 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonUp))
	{
		pos.y += 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonDown))
	{
		pos.y -= 1.0f;
	}
	//g_renderingEngine->SetPointLightPosition(0, pos);
	Vector3 dir = { 1.0f,-1.0f,1.0f };
	dir.Normalize();
	g_renderingEngine->SetSpotLight(0, pos, Vector3{ 10.0f,10.0f,10.0f }, 300.0f, dir, Math::DegToRad(25.0f));

}

void Test::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_bg.Draw(rc);
}
