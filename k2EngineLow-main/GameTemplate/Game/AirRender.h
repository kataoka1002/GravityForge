#pragma once
class AirRender : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// Airの最大数を設定する。
	/// </summary>
	/// <param name="numAir"></param>
	void SetMaxAir(int maxAir)
	{
		m_maxAir = maxAir;
	}
	/// <summary>
	/// インスタンスを除去。
	/// </summary>
	/// <param name="instanceNo"></param>
	void RemoveInstance(int instanceNo)
	{
		m_modelRender.RemoveInstance(instanceNo);
	}
	/// <summary>
	/// インスタンシングデータの更新。
	/// </summary>
	/// <param name="instanceNo">インスタンスの番号</param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateInstancingData(
		int instanceNo,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

private:
	ModelRender	m_modelRender;	// モデルレンダラー。
	int			m_maxAir = 0;	// Airの数
};

