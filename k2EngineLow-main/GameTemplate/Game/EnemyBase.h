#pragma once
class EnemyBase : public IGameObject, Noncopyable
{
public:
	//コンストラクタ
	EnemyBase() {}
	//デストラクタ
	virtual ~EnemyBase() {}

	bool Start();
	virtual void InitModel()				= 0;	//モデルの初期化
	virtual void Render(RenderContext& rc)	= 0;	//描画処理

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

protected:
	ModelRender				m_model;
	Vector3					m_position = Vector3::Zero;					//ポジション
	Vector3					m_scale = Vector3::One;						//大きさ
	Quaternion				m_rotation;									//クォータニオン
};

