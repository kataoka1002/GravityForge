#pragma once

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}


private:
	ModelRender m_bgModel;
	PhysicsStaticObject m_phyStaticObject;	//静的物理オブジェクト
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation;
};