#include "stdafx.h"
#include "Fade.h"
#include "Game.h"
#include "Title.h"

namespace
{
	const float FADE_UP_SPEED = 0.02f;

	const float FADE_DOWN_SPEED = 0.1f;

	const float ROTATION_SPEED = 3.0f;

	const Vector3 LOAD_SPRITE_POSITION = { -640.0f,-320.0f,0.0f };

	const float LOAD_SPRITE_SCALE = 0.7f;
}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//フェード画像の初期化
	m_fadeSprite.Init("Assets/spriteData/title/NowLoading.dds", 1600.0f, 900.0f);
	m_fadeSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
	m_fadeSprite.Update();

	//回転している画像の初期化
	m_loadRotSprite.Init("Assets/spriteData/title/loadRot.dds", 94.0f, 108.0f);
	m_loadRotSprite.SetPosition(LOAD_SPRITE_POSITION);
	m_loadRotSprite.SetScale(LOAD_SPRITE_SCALE);
	m_loadRotSprite.Update();

	return true;
}

void Fade::Update()
{
	//α値の計算
	CalcFadeAlpha();

	//回転処理
	CalcRotation();

	//ゲームクラスの作成
	MakeGame();

	//更新
	m_loadRotSprite.Update();
	m_fadeSprite.Update();
}

void Fade::CalcRotation()
{
	//回転処理
	m_rotDegree += ROTATION_SPEED;
	if (m_rotDegree >= 360.0f)
	{
		m_rotDegree = 0.0f;
	}

	//三角の画像に回転量を教える
	m_loadRot.SetRotationDegZ(m_rotDegree);

	//セット
	m_loadRotSprite.SetRotation(m_loadRot);
}

void Fade::CalcFadeAlpha()
{
	if (m_alphaUp == true)
	{
		//α値を上げていく
		m_fadeSpriteAlpha += FADE_UP_SPEED;
		m_fadeSpriteAlpha = min(1.0f, m_fadeSpriteAlpha);
	}
	else
	{
		//α値を下げていく
		m_fadeSpriteAlpha -= FADE_DOWN_SPEED;
		m_fadeSpriteAlpha = max(0.0f, m_fadeSpriteAlpha);

		//α値が0以下で
		if (m_fadeSpriteAlpha <= 0.0f)
		{
			//自身の削除
			DeleteGO(this);
		}
	}

	m_fadeSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
	m_loadRotSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
}

void Fade::MakeGame()
{
	//α値が1以上になったらゲームを作成する
	if (m_fadeSpriteAlpha >= 1.0f && m_didMakeGame == false)
	{
		Title* title = FindGO<Title>("title");
		title->MakeGame();

		m_didMakeGame = true;
	}
}

void Fade::Render(RenderContext& rc)
{
	m_fadeSprite.Draw(rc);
	m_loadRotSprite.Draw(rc);
}