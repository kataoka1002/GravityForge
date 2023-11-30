#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

namespace
{
	const Vector3 PRESS_A_BUTTON_POSITION = { 0.0f,-300.0f,0.0f };

	const Vector4 GAMESTART_OPTION_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector3 GAMESTART_OPTION_POSITION = Vector3::Zero;

	const Vector4 TRIANGLE_BEFORE_COLOR = { 1.0f,1.0f,1.0f,0.0f };

	const Vector4 TRIANGLE_AFTER_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector3 BGM_SPRITE_POSITION = { -20.0f,-240.0f,0.0f };

	const Vector3 SE_SPRITE_POSITION = { -20.0f,-318.0f,0.0f };

	const Vector3 BGM_FRAME_POSITION = { -30.0f,-240.0f,0.0f };

	const Vector3 SE_FRAME_POSITION = { -30.0f,-318.0f,0.0f };

	const Vector2 BGM_SPRITE_PIVOT = { 0.0f,0.5f };

	const Vector2 SE_SPRITE_PIVOT = { 0.0f,0.5f };

	const float BLINKING_SPEED = 0.1f;

	const float TRIANGLE_MOVE_SPEED = 1.0f;

	const float TRIANGLE_OFFSET = -13.0f;

	const float FADE_SPEED = 0.02f;

	const float PI = 3.141592;
}

Title::~Title()
{

}

bool Title::Start()
{
	//背景の初期化
	m_titleSprite.Init("Assets/spriteData/title/title.dds", 1600.0f, 900.0f);

	//表示されている文字画像の初期化
	m_fontSprite.Init("Assets/spriteData/title/PressAButton.dds", 1600.0f, 900.0f);
	m_fontSprite.SetPosition(PRESS_A_BUTTON_POSITION);
	m_fontSprite.Update();

	//矢印の初期化
	m_triangleSprite.Init("Assets/spriteData/title/GameStartSankaku.dds", 1600.0f, 900.0f);
	m_triangleSprite.SetMulColor(TRIANGLE_BEFORE_COLOR);
	m_triangleSprite.Update();

	//BGM系の初期化
	m_BGMGageSprite.Init("Assets/spriteData/title/SE_BGM_Gage.dds", 319.0f, 36.0f);
	m_BGMGageSprite.SetPosition(BGM_SPRITE_POSITION);
	m_BGMGageSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMGageSprite.Update();
	m_BGMFrameSprite.Init("Assets/spriteData/title/SE_BGM_Frame.dds", 339.0f, 56.0f);
	m_BGMFrameSprite.SetPosition(BGM_FRAME_POSITION);
	m_BGMFrameSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMFrameSprite.Update();
	m_BGMBackSprite.Init("Assets/spriteData/title/BGMSE_Back.dds", 319.0f, 36.0f);
	m_BGMBackSprite.SetPosition(BGM_SPRITE_POSITION);
	m_BGMBackSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMBackSprite.Update();

	//SE系の初期化
	m_SEGageSprite.Init("Assets/spriteData/title/SE_BGM_Gage.dds", 319.0f, 36.0f);
	m_SEGageSprite.SetPosition(SE_SPRITE_POSITION);
	m_SEGageSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEGageSprite.Update();
	m_SEFrameSprite.Init("Assets/spriteData/title/SE_BGM_Frame.dds", 339.0f, 56.0f);
	m_SEFrameSprite.SetPosition(SE_FRAME_POSITION);
	m_SEFrameSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEFrameSprite.Update();
	m_SEBackSprite.Init("Assets/spriteData/title/BGMSE_Back.dds", 319.0f, 36.0f);
	m_SEBackSprite.SetPosition(SE_SPRITE_POSITION);
	m_SEBackSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEBackSprite.Update();

	return true;
}

void Title::Update()
{
	//それぞれのステート時の処理
	ManageState();

	//三角を動かす
	MoveTriangle();

	//BGM.SEの大きさの設定
	m_BGMGageSprite.SetScale({ m_BGMScale, 1.0f, 1.0f });
	m_BGMGageSprite.Update();
	m_SEGageSprite.SetScale({ m_SEScale, 1.0f, 1.0f });
	m_SEGageSprite.Update();
}

void Title::ManageState()
{
	switch (m_titleState)
	{
	case enTitleState_PressAButton:

		//文字の点滅の処理
		PressAButtonFade();

		//Aボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ゲームスタートオプション画面へ
			ToGameStartOption();

			//矢印をゲームスタートへ
			TriangleChange("Assets/spriteData/title/GameStartSankaku.dds", enTitleState_GameStart);
		}

		break;

	case enTitleState_GameStart:

		//Aボタンでゲームスタート
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//フェードステートに変更
			m_titleState = enTitleState_Fade;
		}

		//下ボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//矢印をオプションへ
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		break;

	case enTitleState_Option:

		//上ボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//矢印をゲームスタートへ
			TriangleChange("Assets/spriteData/title/GameStartSankaku.dds", enTitleState_GameStart);
		}

		// Aボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//BGM,SE設定画面へ
			ToBGMSE();

			//矢印をBGMへ
			TriangleChange("Assets/spriteData/title/BGMSankaku.dds", enTitleState_BGM);
		}

		break;

	case enTitleState_BGM:

		//下ボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//矢印をSEへ
			TriangleChange("Assets/spriteData/title/SESankaku.dds", enTitleState_SE);
		}

		//Bボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ゲームスタートオプション画面へ
			ToGameStartOption();

			//矢印をオプションへ
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		//BGMの大きさ設定
		ChangeBGMSEScale(m_BGMScale);

		break;

	case enTitleState_SE:

		//上ボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			TriangleChange("Assets/spriteData/title/BGMSankaku.dds", enTitleState_BGM);
		}

		//Bボタンでステート変更
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ゲームスタートオプション画面へ
			ToGameStartOption();

			//矢印をオプションへ
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		//SEの大きさ設定
		ChangeBGMSEScale(m_SEScale);

		break;

	case enTitleState_Fade:
		//フェードを行う
		CalcFade();
		break;

	default:
		break;
	}
}

void Title::ToGameStartOption()
{
	//ゲームスタートオプション画面に変更
	m_fontSprite.Init("Assets/spriteData/title/GameStart_Option.dds", 1600.0f, 900.0f);
	m_fontSprite.SetMulColor(GAMESTART_OPTION_COLOR);
	m_fontSprite.SetPosition(GAMESTART_OPTION_POSITION);
	m_fontSprite.Update();

	//三角の透明度を上げる
	m_triangleSprite.SetMulColor(TRIANGLE_AFTER_COLOR);
	m_triangleSprite.Update();
}

void Title::ToBGMSE()
{
	//SE,BGM画面に変更
	m_fontSprite.Init("Assets/spriteData/title/SE_BGM.dds", 1600.0f, 900.0f);
	m_fontSprite.Update();

	//オプションステートに変更
	m_titleState = enTitleState_BGM;
}

void Title::TriangleChange(const char* name, enTitleState state)
{
	//三角を移動
	m_triangleSprite.Init(name, 1600.0f, 900.0f);
	m_triangleSprite.Update();

	//ステート変更
	m_titleState = state;
}

void Title::ChangeBGMSEScale(float& scale)
{
	if (g_pad[0]->IsPress(enButtonRight))
	{
		scale += 0.01;
	}
	if (g_pad[0]->IsPress(enButtonLeft))
	{
		scale -= 0.01;
	}

	//大きさは0～1
	scale = max(0.0f, scale);
	scale = min(1.0f, scale);
}

float Title::CalcSinValue(float speed)
{
	//sin波を使って滑らかに上下させる
	//角度を大きくしていく
	m_degree += speed;

	//360度で-1～1を一周するので0度に戻す
	if (m_degree >= 180.0f)
	{
		m_degree = 0.0f;
	}

	//角度をラジアンに変換
	float rad = m_degree * PI / 180.0f;

	//sinの値を求め,それを透明度にする
	float sinValue = sin(rad);
	return sinValue;
}

void Title::PressAButtonFade()
{
	//消えたり出たりする
	float fontAlpha = CalcSinValue(BLINKING_SPEED);

	m_fontSprite.SetMulColor({ 1.0f,1.0f,1.0f,fontAlpha });
	m_fontSprite.Update();
}

void Title::MoveTriangle()
{
	//矢印を横に動かす
	m_trianglePosX = CalcSinValue(TRIANGLE_MOVE_SPEED) * TRIANGLE_OFFSET;

	m_triangleSprite.SetPosition({ m_trianglePosX ,0.0f,0.0f });
	m_triangleSprite.Update();
}

void Title::CalcFade()
{
	//フェードが作成されていなかったら
	if (m_makeFade == false)
	{
		//フェードの作成
		NewGO<Fade>(0, "fade");
		m_makeFade = true;
	}
}

void Title::MakeGame()
{
	//ゲームを作成
	NewGO<Game>(0, "game");

	//自分自身の削除
	DeleteGO(this);
}

void Title::Render(RenderContext& rc)
{
	m_titleSprite.Draw(rc);
	m_fontSprite.Draw(rc);
	m_triangleSprite.Draw(rc);

	//BGM,SEの時のみ表示
	if (m_titleState == enTitleState_BGM || m_titleState == enTitleState_SE)
	{
		m_BGMBackSprite.Draw(rc);
		m_BGMGageSprite.Draw(rc);
		m_BGMFrameSprite.Draw(rc);
		m_SEBackSprite.Draw(rc);
		m_SEGageSprite.Draw(rc);
		m_SEFrameSprite.Draw(rc);
	}
}