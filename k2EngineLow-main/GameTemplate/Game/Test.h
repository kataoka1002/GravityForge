#pragma once
class Test : public IGameObject
{
public:
	Test();
	~Test();

	void Update();

	void Render(RenderContext& rc);

private:
	ModelRender m_model;

	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Num,
	};
	AnimationClip animationClips[enAnimClip_Num];

};

