#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Num,
	};
	AnimationClip animationClips[enAnimClip_Num];

	ModelRender m_playerModel;
};
