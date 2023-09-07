#include "stdafx.h"
#include "Test.h"

Test::Test()
{
	m_model.Init("Assets/modelData/unityChan.tkm");
	m_model.SetPosition({ 0.0f,0.0f,0.0f });
}

Test::~Test()
{

}

void Test::Update()
{
	m_model.Update();
}

void Test::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
