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
		m_bgModel.SetPosition(pos);
	}

	void SetScale(const Vector3 scale)
	{
		m_bgModel.SetScale(scale);
	}

	void SetRotation(const Quaternion rot)
	{
		m_bgModel.SetRotation(rot);
	}


private:
	ModelRender m_bgModel;
	PhysicsStaticObject m_phyStaticObject;	//静的物理オブジェクト
};