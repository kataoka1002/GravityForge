#pragma once
#include "EnemyBase.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	//前方宣言
	class IHumanEnemyState;

	class HumanEnemy : public EnemyBase
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
			enAnimClip_Num
		};

	public:
		HumanEnemy();
		~HumanEnemy();

		void Update();								//更新処理
		void PlayAnimation(EnAnimationClip currentAnimationClip);	//アニメーションの再生
		void SetCollision();						//コリジョンの設定
		void MoveCollision();						//コリジョンの動きの設定	
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// アニメーションイベント用の関数。
		void FollowPlayer()				override;	//動きの処理
		void Turn()						override;	//回転処理
		void OnDestroy()				override;	//消えるときに呼ばれる処理
		void InitModel()				override;	//モデルの初期化
		void Render(RenderContext& rc)	override;	//描画処理

		/// <summary>
		/// 再生するアニメーションクリップを設定する。
		/// </summary>
		/// <param name="enAnimationClip">アニメーションクリップ</param>
		/// <param name="complementTime">補間時間</param>
		void SetAnimation(const EnAnimationClip enAnimationClip, const float complementTime)
		{
			m_currentAnimationClip = enAnimationClip;
			m_complementTime = complementTime;
		}

		/// <summary>
		/// キャラコンの再設定
		/// </summary>
		void ResetCharaCon()
		{
			m_charaCon.RemoveRigidBoby();
			m_charaCon.Init(
				30.0f,			//半径
				10.0f,			//高さ
				m_position		//座標
			);
		}

		/// <summary>
		/// アニメーション再生中かどうかの取得
		/// </summary>
		/// <returns></returns>
		const bool& GetIsPlayingAnimation() const
		{
			return m_model.IsPlayingAnimation();
		}

		/// <summary>
		/// 体力の取得
		/// </summary>
		/// <returns></returns>
		const float GetHP()const
		{
			return m_hp;
		}

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// アニメーションクリップ
		EnAnimationClip		m_currentAnimationClip;					// 現在設定されているアニメーションクリップ
		IHumanEnemyState*	m_humanEnemyState = nullptr;			// ステート	
		float				m_complementTime = 0.0f;				// アニメーションの補完時間
	};
}
