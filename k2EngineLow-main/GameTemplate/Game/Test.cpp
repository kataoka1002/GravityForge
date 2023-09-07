#include "stdafx.h"
#include "Test.h"

Test::Test()
{
	animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);

	m_model.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimClip_Num, enModelUpAxisY);

	g_renderingEngine->SetDirectionLight(0, Vector3{ 0.0f,0.0f,1.0f }, Vector3{ 1.0f,0.0f,0.0f });
	g_renderingEngine->SetDirectionLight(1, Vector3{ 0.0f,0.0f,1.0f }, Vector3{ 0.0f,1.0f,0.0f });
}

Test::~Test()
{

}

void Test::Update()
{
	m_model.PlayAnimation(enAnimClip_Idle);
	m_model.Update();
}

void Test::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
