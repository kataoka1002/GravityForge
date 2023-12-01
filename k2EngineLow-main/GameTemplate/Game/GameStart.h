#pragma once
class GameInformation;

class GameStart : public IGameObject
{
public:
	~GameStart();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_gameStartSprite;
	float m_timer = 0.0f;
	float m_alpha = 0.0f;

	bool m_alphaUP = true;
	bool m_alphaDown = false;

	GameInformation* m_gameInfo = nullptr;
};

