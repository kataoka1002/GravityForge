#pragma once
//前方宣言
class GameCamera;
class Game;
class ObjectBase;
class Teapot;

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	//前方宣言
	class IPlayerState;

	class Player : public IGameObject
	{
	public:
		//アニメーションクリップ
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Walk,
			enAnimClip_Jump,
			enAnimClip_Attract,
			enAnimClip_Standby,
			enAnimClip_Standwalk,
			enAnimClip_Attack,
			enAnimClip_WalkRight,
			enAnimClip_WalkLeft,
			enAnimClip_WalkBack,
			enAnimClip_WalkJump,
			enAnimClip_Reaction,
			enAnimClip_Dead,
			enAnimClip_Punch,
			enAnimClip_Num
		};

	public:
		Player();
		~Player();
		bool Start();
		void Update();
		void Render(RenderContext& rc);
		void Move();
		void Turn();
		void Attract();
		void Attack();
		void ResetAttack();
		void ChangeWalkingStyle();
		void PlayAnimation(EnAnimationClip currentAnimationClip);
		bool DidAttackHit();
		void CalcDamage(float damage);
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// アニメーションイベント用の関数
		void GameOver();

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
		/// オブジェクトを保持しているかどうかを設定する
		/// </summary>
		/// <param name="isHold"></param>
		void SetIsHoldingObject(const bool isHold)
		{
			m_isHoldingObject = isHold;
		}

		/// <summary>
		/// 引き寄せることが出来るかを設定する
		/// </summary>
		/// <param name="canAtt"></param>
		void SetCanAttract(const bool canAtt)
		{
			m_canAttract = canAtt;
		}

		/// <summary>
		/// 攻撃することが出来るかを設定する
		/// </summary>
		/// <param name="canAtt"></param>
		void SetCanAttack(const bool canAtt)
		{
			m_canAttack = canAtt;
		}

		/// <summary>
		/// Y方向に力を加えてジャンプする
		/// </summary>
		void SetJump()
		{
			m_moveSpeed.y = 400.0f;
		}

		/// <summary>
		/// ジャンプ中かどうかをセットする
		/// </summary>
		/// <param name="junping"></param>
		void SetIsJumping(bool junping)
		{
			m_isJumping = junping;
		}

		/// <summary>
		/// ポジションの取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// 回転量の取得
		/// </summary>
		/// <returns></returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// プレイヤーの速さの取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPlayerSpeed()const
		{
			return m_moveSpeed;
		}

		/// <summary>
		/// オブジェクトを持っているかどうかの取得
		/// </summary>
		/// <returns></returns>
		const bool& GetIsHoldingObject()const
		{
			return m_isHoldingObject;
		}

		/// <summary>
		/// アニメーション再生中かどうかの取得
		/// </summary>
		/// <returns></returns>
		const bool& GetIsPlayingAnimation() const
		{
			return m_playerModel.IsPlayingAnimation();
		}

		/// <summary>
		/// 引き寄せることが出来るかどうかの取得
		/// </summary>
		/// <returns></returns>
		const bool& GetCanAttract()const
		{
			return m_canAttract;
		}

		/// <summary>
		/// 攻撃できるかどうかの取得
		/// </summary>
		/// <returns></returns>
		const bool& GetCanAttack()const
		{
			return m_canAttack;
		}

		/// <summary>
		/// 地面に着いているかを判定
		/// </summary>
		/// <returns></returns>
		const bool& GetOnGround()const
		{
			return m_charaCon.IsOnGround();
		}

		/// <summary>
		/// プレイヤーのHPの取得
		/// </summary>
		/// <returns></returns>
		const float& GetHP()const
		{
			return m_hp;
		}

		/// <summary>
		/// HPの割合を取得
		/// </summary>
		/// <returns></returns>
		const float GetHPRatio()const
		{
			return m_hp / 100.0f;
		}


	private:

		AnimationClip		animationClips[enAnimClip_Num];			// アニメーションクリップ
		EnAnimationClip		m_currentAnimationClip;					// 現在設定されているアニメーションクリップ
		ModelRender			m_playerModel;							// プレイヤーモデル
		Vector3				m_position = Vector3::Zero;				// 座標
		Vector3				m_scale = Vector3::One;					// スケール
		Vector3				m_moveSpeed = Vector3::Zero;			// 移動速度
		Vector3             m_rotMove = Vector3::Zero;				// 回転速度
		Vector3				m_forward = Vector3::Zero;				// 前方向
		CharacterController m_charaCon;								// キャラクターコントローラー
		Quaternion			m_rotation;								// クォータニオン
		float				LStick_x = 0.0f;						// Lスティック入力量(横)
		float				LStick_y = 0.0f;						// Lスティック入力量(縦)
		float				m_complementTime = 0.0f;				// アニメーションの補完時間
		float				m_hp = 0.0f;							// HP
		bool				m_isHoldingObject = false;				// オブジェクトを持っているかどうか
		bool				m_canAttract = false;					// 引き寄せられるかどうか
		bool				m_canAttack = false;					// 攻撃できるかどうか
		bool				m_isJumping = false;					// その場ジャンプ中かどうか

		Game* m_game = nullptr;
		GameCamera* m_camera = nullptr;
		ObjectBase* m_holdingObject = nullptr;
		IPlayerState* m_playerState = nullptr;
	};
}