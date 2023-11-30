#pragma once
class Fade : public IGameObject
{
public:
	~Fade();
	bool Start();
	void Update();
	void CalcRotation();
	void CalcFadeAlpha();
	void MakeGame();
	void Render(RenderContext& rc);

	void DeleteThis()
	{
		DeleteGO(this);
	}	

	void SetAlphaDown()
	{
		m_alphaUp = false;
	}

private:
	SpriteRender m_fadeSprite;				//フェード用画像
	SpriteRender m_loadRotSprite;			//回っている画像

	float m_fadeSpriteAlpha = 0.0f;			//フェード画面のα
	float m_rotDegree = 0.0f;				//ロードの回転量
	bool m_didMakeGame = false;				//ゲームクラスが作られているかどうか
	bool m_alphaUp = true;					//α値を上げるかどうか

	Quaternion m_loadRot;					//最終的な回転量
};

