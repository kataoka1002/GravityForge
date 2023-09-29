#pragma once
#include "SkyCube.h"

class Test : public IGameObject
{
public:
	Test();
	~Test();
	bool Start();
	void Update();

	void Render(RenderContext& rc);

private:
	ModelRender m_model;
	ModelRender m_bg;
	ModelRender m_guitar;
	SkyCube* m_skyCube;
	Quaternion qRot;

	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Num,
	};
	AnimationClip animationClips[enAnimClip_Num];

	Vector3 pos = {0.0f,0.0f,-50.0f};
	Vector3 pow = { 0.0f,0.0f,0.0f };

	SpriteRender m_sprite;
	FontRender m_font;
};

