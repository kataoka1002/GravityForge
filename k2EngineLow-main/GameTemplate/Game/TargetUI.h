#pragma once

class TargetUI : public IGameObject
{
public:
	~TargetUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetObjectPosition(const Vector3& objPos)
	{
		m_objectPos = objPos;
	}

private:
	SpriteRender m_yellowSprite;
	SpriteRender m_blueSprite;
	SpriteRender m_circleSprite;

	Quaternion m_rotYellow;
	Quaternion m_rotBlue;
	float m_degree = 0.0f;
	bool m_didPositionUP = false;
	Vector2 m_targetUIPosition = Vector2::Zero;
	Vector3 m_objectPos = Vector3::Zero;
};

