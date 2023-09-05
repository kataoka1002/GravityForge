#include "stdafx.h"
#include "Test.h"

Test::Test()
{
	m_model.Init("Assets/modelData/unityChan.tkm");
}

Test::~Test()
{

}

void Test::Update()
{
	m_model.Update();
}

void Test::Draw(RenderContext& rc)
{
	m_model.Draw(rc);
}
