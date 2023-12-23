#pragma once

class PlantLowRender : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 最大数を設定する。
	/// </summary>
	/// <param name="numModel"></param>
	void SetMaxModel(int maxModel)
	{
		m_maxModel = maxModel;
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
		const Vector3& scale)
	{
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

private:
	ModelRender	m_modelRender;	// モデルレンダラー
	int			m_maxModel = 0;	// モデルの数
};