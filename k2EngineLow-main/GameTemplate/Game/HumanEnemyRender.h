#pragma once

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	class HumanEnemyRender : public IGameObject
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Crawl,
			enAnimClip_Tremble,
			enAnimClip_Die,
			enAnimClip_Walk,
			enAnimClip_Attack,
			enAnimClip_Reaction,
			enAnimClip_Num
		};

		bool Start();
		void Update();
		void Render(RenderContext& rc);

		/// <summary>
		/// 最大数を設定する。
		/// </summary>
		/// <param name="numHumanEnemy"></param>
		void SetMaxHumanEnemy(int maxHumanEnemy)
		{
			m_maxHumanEnemy = maxHumanEnemy;
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
		ModelRender			m_modelRender;								// モデルレンダラー
		int					m_maxHumanEnemy = 0;						// モデルの数
		AnimationClip		animationClips[enAnimClip_Num];				// アニメーションクリップ
		EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// 現在設定されているアニメーションクリップ
		float				m_complementTime = 0.0f;					// アニメーションの補完時間
	};
}