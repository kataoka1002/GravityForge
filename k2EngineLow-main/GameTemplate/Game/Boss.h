#pragma once
#include "EnemyBase.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	//前方宣言
	class IBossState;
	class BossHitReactionState;
	class BossUI;

	class Boss : public EnemyBase
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Reaction,
			enAnimClip_Jump,
			enAnimClip_Magic,
			enAnimClip_Punch,
			enAnimClip_Swipe,
			enAnimClip_Walk,
			enAnimClip_JumpAttack,
			enAnimClip_Num
		};

	public:
		Boss();
		~Boss();

		void Update();								//更新処理
		void PlayAnimation(EnAnimationClip currentAnimationClip);	//アニメーションの再生
		void SetCollision();						//コリジョンの設定
		void MoveCollision();						//コリジョンの動きの設定	
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// アニメーションイベント用の関数
		void SetReactionState();
		void CheckHP();
		void GameClear();							//ゲームをクリアした時に呼ばれる
		void CalcMagicCoolDown();					//マジックのクールダウンタイムの計算
		void CalcHitCoolDown();						//ヒットのクールダウンタイムの計算
		void JumpAttack();
		void Gravity(float pow);
		void SetPlate();
		void MagicAttack();
		void WalkSEProcess();
		bool ToPlayerVecCheck();
		void FollowPlayer()				override;	//動きの処理
		void Turn()						override;	//回転処理
		void OnDestroy()				override;	//消えるときに呼ばれる処理
		void InitModel()				override;	//モデルの初期化
		void PlayReaction()				override;	//ヒューマンエネミー用なのでここでは特に意味なし
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
		/// HPの割合を取得
		/// </summary>
		/// <returns></returns>
		const float GetHPRatio()const
		{
			return m_hp / 100.0f;
		}

		/// <summary>
		/// マジックのクールダウンタイム中かどうかを返す
		/// </summary>
		/// <returns></returns>
		bool GetIsMagicCoolDown()
		{
			return m_isMagicCoolDowm;
		}

		/// <summary>
		/// マジックのクールダウンタイム中にする
		/// </summary>
		void SetMagicCoolDown()
		{
			m_isMagicCoolDowm = true;
		}

		/// <summary>
		/// ヒットのクールダウンタイム中かどうかを返す
		/// </summary>
		/// <returns></returns>
		bool GetIsHitCoolDown()
		{
			return m_isHitCoolDowm;
		}

		/// <summary>
		/// ヒットのクールダウンタイム中にする
		/// </summary>
		void SetHitCoolDown()
		{
			m_isHitCoolDowm = true;
		}

		/// <summary>
		/// ジャンプ攻撃のターゲットを設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetJumpTarget()
		{
			m_jumpTargetPos = m_player->GetPosition();
		}

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// アニメーションクリップ
		EnAnimationClip		m_currentAnimationClip;					// 現在設定されているアニメーションクリップ
		IBossState*			m_bossState = nullptr;					// ステート	
		float				m_complementTime = 0.0f;				// アニメーションの補完時間
		float				m_magicCoolDowmTime = 0.0f;				// マジックのクールダウンタイム
		bool				m_isMagicCoolDowm = false;				// マジックのクールダウンタイムかどうか
		float				m_hitCoolDowmTime = 0.0f;				// ヒットのクールダウンタイム
		bool				m_isHitCoolDowm = false;				// ヒットのクールダウンタイムかどうか
		bool				m_isJumping = false;					// ジャンプ中かどうか
		Vector3				m_jumpTargetPos = Vector3::Zero;		// ジャンプ攻撃の目的地
		float				m_plateDrawTime = 2.0f;
		bool				m_magicAttackNow = false;				// マジック中かどうか
		bool				m_makeMagicEffe = false;				// マジックのエフェクトを作ったかどうか
		bool				m_isPlayNameSE = false;					// 名前の効果音を再生したかどうか
		bool				m_isWalking = false;					// 歩いているかどうか

		BossUI*				m_ui = nullptr;
		SoundSource*		m_walkSE = nullptr;						// 足音
	};
}
