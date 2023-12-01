#include "stdafx.h"
#include "BlackFade.h"

BlackFade::~BlackFade()
{

}

bool BlackFade::Start()
{
	m_blackSprite.Init("Assets/spriteData/title/title.dds", 1600.0f, 900.0f);
	m_blackSprite.SetMulColor({ 0.0f,0.0f,0.0f,0.0f });
	m_blackSprite.Update();

	return true;
}

void BlackFade::Update()
{
	//ƒ¿’l‚ÌŒvŽZ
	CalcAlpha();

	m_blackSprite.Update();
}

void BlackFade::CalcAlpha()
{
	if (m_alphaUp == true)
	{
		m_blackAlpha += 0.05f;
	}

	if (m_alphaDown == true)
	{
		m_blackAlpha -= 0.05f;
	}

	m_blackSprite.SetMulColor({ 0.0f,0.0f,0.0f,m_blackAlpha });
}

void BlackFade::Render(RenderContext& rc)
{
	m_blackSprite.Draw(rc);
}