#pragma once
#include "EnemyBase.h"
class EnemyUI;

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	//前方宣言
	class IHumanEnemyState;
	class HumanEnemyRender;

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
			enAnimClip_Reaction,
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
		void HandleAttackHit();
		void CheckHP();
		void CheckDistance();
		void FollowPlayer()				override;	//動きの処理
		void Turn()						override;	//回転処理
		void OnDestroy()				override;	//消えるときに呼ばれる処理
		void InitModel()				override;	//モデルの初期化
		void PlayReaction()				override;
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

		/// <summary>
		/// プレイヤーまでの距離を測る
		/// </summary>
		/// <returns></returns>
		float CheckDistanceToPlayer()
		{
			//プレイヤーまでのベクトルを求め距離を求める
			Vector3 toPlayer = m_player->GetPosition() - m_position;
			float toPlayerLen = toPlayer.Length();
			return toPlayerLen;
		}

		/// <summary>
		/// インスタンス番号を設定。
		/// </summary>
		/// <param name="instanceNo"></param>
		void SetInstanceNo(int instanceNo)
		{
			m_instanceNo = instanceNo;
		}

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// アニメーションクリップ
		EnAnimationClip		m_currentAnimationClip;					// 現在設定されているアニメーションクリップ
		IHumanEnemyState*	m_humanEnemyState = nullptr;			// ステート	
		float				m_complementTime = 0.0f;				// アニメーションの補完時間
		int					m_instanceNo = 0;						// インスタンス番号。
		bool				m_isSetDeadState = false;				// 半死にステートをセットしたか
		bool				m_isSetDieState = false;				// 死亡ステートをセットしたか
		bool				m_isWithinRange = false;
		EnemyUI*			m_enemyUI = nullptr;
		HumanEnemyRender*   m_humanEnemyRender = nullptr;
		SphereCollider		m_sphereCollider;						//遮蔽物確認用のコライダー
	};
}
