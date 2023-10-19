#pragma once
#include "SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Lighting.h"
#include "Teapot.h"
#include "Reticle.h"

class Game : public IGameObject, Noncopyable
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetSkyCube();

private:
	SkyCube* m_skyCube = nullptr;
	Player* m_player = nullptr;
	BackGround* m_bg = nullptr;
	GameCamera* m_camera = nullptr;
	Lighting* m_light = nullptr;
	Teapot* m_teapot = nullptr;
	Reticle* m_reticle = nullptr;
};

