#pragma once
#include "Player.h"
class Game;

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
	virtual void FollowPlayer()				= 0;	//移動処理
	virtual void Turn()						= 0;	//回転処理
	virtual void OnDestroy()				= 0;	//消えるときに呼ばれる処理
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

protected:
	ModelRender				m_model;
	Vector3					m_position = Vector3::Zero;				// ポジション
	Vector3					m_scale = Vector3::One;					// 大きさ
	Quaternion				m_rotation;								// クォータニオン
	CharacterController		m_charaCon;								// キャラクターコントローラー
	float					m_hp = 0.0f;							// HP
	Vector3					m_forward = {0.0f,0.0f,1.0f};			// 前方向
	Vector3					m_moveSpeed = Vector3::Zero;			// 移動速度
	Vector3					m_rotMove = Vector3::Zero;				// 回転速度

	Game*					m_game = nullptr;
	nsPlayer::Player*		m_player = nullptr;
	CollisionObject*		m_collisionObject = nullptr;			// コリジョン
};

