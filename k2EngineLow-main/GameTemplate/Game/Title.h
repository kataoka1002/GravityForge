#pragma once

class Title : public IGameObject
{
public:
	enum enTitleState
	{
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
	void Fade();

private:
	enTitleState m_titleState = enTitleState_PressAButton;	//�X�e�[�g
	SpriteRender m_titleSprite;								//�^�C�g���̉摜
	SpriteRender m_fontSprite;								//�����̉摜
	SpriteRender m_triangleSprite;							//���̉摜
	SpriteRender m_BGMGageSprite;							//BGM�̃Q�[�W
	SpriteRender m_BGMFrameSprite;							//BGM�̃t���[��
	SpriteRender m_BGMBackSprite;							//BGM�̔w�i
	SpriteRender m_SEGageSprite;							//SE�̃Q�[�W
	SpriteRender m_SEFrameSprite;							//SE�̃t���[��
	SpriteRender m_SEBackSprite;							//SE�̔w�i
	SpriteRender m_fadeSprite;								//�t�F�[�h�p�摜

	float m_degree = 0.0f;									//�����x�̕ύX�p�p�x
	float m_BGMScale = 1.0f;								//BGM�̑傫��
	float m_SEScale = 1.0f;									//SE�̑傫��
	float m_trianglePosX = 0.0f;							//����X���W
	float m_fadeSpriteAlpha = 0.0f;							//�t�F�[�h��ʂ̃�
};
