#include "stdafx.h"
#include "Test.h"

Test::Test()
{
	m_sprite.Init("Assets/modelData/utc_nomal.dds", 100.0f, 100.0f);
	m_sprite.SetPosition({ -100.0f,50.0f,0.0f });

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


	//ìGÇéEÇµÇΩêîÇÃï\é¶
	wchar_t text[256];
	swprintf_s(text, 256, L"%02dÅ@/%02d", 20, 55);
	m_font.SetPivot({ 0.0f,0.5f });
	m_font.SetText(text);
	m_font.SetPosition(Vector3(100, 100.0f, 0.0f));
	m_font.SetShadowParam(true, 1.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_font.SetScale(2.5f);
	m_font.SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_sprite.Update();
}

void Test::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_bg.Draw(rc);
	m_sprite.Draw(rc);
	m_font.Draw(rc);
}
