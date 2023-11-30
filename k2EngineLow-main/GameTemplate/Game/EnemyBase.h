#pragma once
#include "Player.h"
class Game;
class GameInformation;

class EnemyBase : public IGameObject, Noncopyable
{
public:
	//コンストラクタ
	EnemyBase() {}
	//デストラクタ
	virtual ~EnemyBase() {}

	bool Start();
	void HandleDamageEvent(float damage);			//ダメージを受けた時の処理
	bool DidAttackHit();
	void CalcCoolDown();
	virtual void FollowPlayer()				= 0;	//移動処理
	virtual void Turn()						= 0;	//回転処理
	virtual void OnDestroy()				= 0;	//消えるときに呼ばれる処理
	virtual void InitModel()				= 0;	//モデルの初期化
	virtual void PlayReaction() = 0;
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

	/// <summary>
	/// キャラコンを返す
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}

	/// <summary>
	/// コリジョンを返す
	/// </summary>
	/// <returns></returns>
	CollisionObject* GetCollision()
	{
		return m_collisionObject;
	}

	/// <summary>
	/// ポジションを返す
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 今のHPを返す
	/// </summary>
	/// <returns></returns>
	const float& GetHP()
	{
		return m_hp;
	}

	/// <summary>
	/// HPの最大値を返す
	/// </summary>
	/// <returns></returns>
	const float& GetMaxHP()
	{
		return m_hpMax;
	}

	/// <summary>
	/// クロール中にする
	/// </summary>
	void SetCrawlNow()
	{
		m_isCrawling = true;
	}

	/// <summary>
	/// クロール中かどうかを返す
	/// </summary>
	/// <returns></returns>
	bool IsCrawling()
	{
		return m_isCrawling;
	}


protected:
	ModelRender				m_model;
	Vector3					m_position = Vector3::Zero;				// ポジション
	Vector3					m_scale = Vector3::One;					// 大きさ
	Quaternion				m_rotation;								// クォータニオン
	CharacterController		m_charaCon;								// キャラクターコントローラー
	float					m_hp = 0.0f;							// HP
	float					m_hpMax = 0.0f;							// HPの最大値
	Vector3					m_forward = {0.0f,0.0f,1.0f};			// 前方向
	Vector3					m_moveSpeed = Vector3::Zero;			// 移動速度
	Vector3					m_rotMove = Vector3::Zero;				// 回転速度
	bool					m_isCrawling = false;					// クロール中かどうか
	bool					m_isCooldown = false;					// クールダウン中かどうか
	float					m_cooldownTime = 1.0f;						// 攻撃のクールダウン

	Game*					m_game = nullptr;
	nsPlayer::Player*		m_player = nullptr;
	CollisionObject*		m_collisionObject = nullptr;			// コリジョン
	GameInformation* m_gameInfo = nullptr;
};

