#pragma once
class Wall : public IGameObject
{
public:
	~Wall();
	bool Start();
	void Update();

	/// <summary>
	/// ポジションの設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}

private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation;
	CollisionObject* m_collisionObject = nullptr;			// コリジョン
};

