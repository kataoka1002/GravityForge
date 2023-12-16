#pragma once

class Title : public IGameObject
{
public:
	enum enTitleState
	{
		enTitleState_OpenFade,
		enTitleState_PressAButton,
		enTitleState_GameStart,
		enTitleState_Option,
		enTitleState_BGM,
		enTitleState_SE,
		enTitleState_Fade,
		enTitleState_Num
	};

public:
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void ManageState();
	float CalcSinValue(float speed);
	void PressAButtonFade();
	void ToGameStartOption();
	void ToBGMSE();
	void TriangleChange(const char* name, enTitleState state);
	void ChangeBGMSEScale(float& scale);
	void MoveTriangle();
	void CalcFade();
	void MakeGame();

private:
	enTitleState m_titleState = enTitleState_OpenFade;		//ステート
	SpriteRender m_openFadeSprite;
	SpriteRender m_titleSprite;								//タイトルの画像
	SpriteRender m_fontSprite;								//文字の画像
	SpriteRender m_triangleSprite;							//矢印の画像
	SpriteRender m_BGMGageSprite;							//BGMのゲージ
	SpriteRender m_BGMFrameSprite;							//BGMのフレーム
	SpriteRender m_BGMBackSprite;							//BGMの背景
	SpriteRender m_SEGageSprite;							//SEのゲージ
	SpriteRender m_SEFrameSprite;							//SEのフレーム
	SpriteRender m_SEBackSprite;							//SEの背景

	float m_degree = 0.0f;									//透明度の変更用角度
	float m_BGMScale = 1.0f;								//BGMの大きさ
	float m_SEScale = 1.0f;									//SEの大きさ
	float m_trianglePosX = 0.0f;							//矢印のX座標
	float m_openAlpha = 1.0f;								//最初の画像のα値
	bool m_makeFade = false;								//フェードが作られているかどうか

	SoundSource* m_titleBGM = nullptr;						//タイトルのBGM
};

