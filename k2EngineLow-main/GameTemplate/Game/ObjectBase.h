#pragma once
// 前方宣言。
namespace nsPlayer { class Player; }
class Game;
class Boss;

class ObjectBase : public IGameObject, Noncopyable
{
public:

	//コンストラクタ
	ObjectBase() {}
	//デストラクタはバーチャルにしておく
	virtual ~ObjectBase() {}
	bool Start();
	/// <summary>
	/// 引き寄せ処理の初期化
	/// </summary>
	void InitAttract();

	/// <summary>
	/// ぶっ飛び処理の初期化
	/// </summary>
	void InitBlowAway();

	virtual void Render(RenderContext& rc)	= 0;	//描画処理
	virtual void InitModel()				= 0;	//モデルの初期化
	virtual void OnDestroy()				= 0;	//消えるときに呼ばれる処理
	virtual void InitCollision()			= 0;	//コリジョンの初期化

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
	/// 目的地の設定
	/// </summary>
	/// <param name="pos"></param>
	void SetTargetPos(const Vector3 pos)
	{
		 m_targetPosition = pos;
	}

	/// <summary>
	/// 速さの設定
	/// </summary>
	/// <param name="vel"></param>
	void SetVelocity(Vector3 vel)
	{
		m_velocity = vel;
	}

	/// <summary>
	/// 引き寄せれるかどうかの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetCanAttract() const
	{
		return m_canAttract;
	}
	
protected:

	/// <summary>
	/// プレイヤーまで引き寄せられる処理
	/// </summary>
	void AttractedToPlayer();

	/// <summary>
	/// 引き寄せ中の速度計算
	/// </summary>
	/// <param name="speed"></param>
	/// <param name="curvatureRadius"></param>
	/// <param name="damping"></param>
	void CalcVelocity(const float speed, const float curvatureRadius, const float damping);

	/// <summary>
	/// 動きの処理
	/// </summary>
	void Move();

	/// <summary>
	/// アイドル中の処理
	/// </summary>
	void IdleMove();

	/// <summary>
	/// 追尾中の処理
	/// </summary>
	void FollowPlayer();

	/// <summary>
	/// フワフワ処理
	/// </summary>
	void UpDown();

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="speed"></param>
	void Turn(Vector3 speed);

	/// <summary>
	/// アイドル中の目的地の計算
	/// </summary>
	void CalcTargetPosition();

	/// <summary>
	/// 吹っ飛ぶ処理
	/// </summary>
	void BlowAway();

	/// <summary>
	/// 物に当たっているかの処理
	/// </summary>
	void CalcCollision();

	/// <summary>
	/// レティクルに合わさっているかの処理
	/// </summary>
	void CalcAimingDirection();

protected:
	enum EnObjectState
	{
		enObjectState_Quiescence,	//初期状態
		enObjectState_Idle,			//待機中
		enObjectState_Attract,		//引き寄せ中
		enObjectState_Blow,
		enObjectState_Num
	};

	ModelRender				m_model;									//モデル
	CharacterController		m_charaCon;									//キャラクターコントローラー
	SphereCollider			m_sphereCollider;							//遮蔽物確認用のコライダー
	Vector3					m_position = Vector3::Zero;					//ポジション
	Vector3					m_targetPosition = Vector3::Zero;			//目的地
	Vector3					m_velocity = Vector3::Zero;					//速度
	Vector3					m_rotMove = Vector3::Zero;					//回転速度
	Vector3					m_flightSpeed = Vector3::Zero;				//飛んでく速さ
	Vector3					m_followSpeed = Vector3::Zero;				//追尾の速さ
	Vector3					m_scale = Vector3::Zero;					//大きさ
	Vector3					m_collisionPosition = Vector3::Zero;		//コリジョンオブジェクトの座標
	Quaternion				m_rotation;									//クォータニオン
	EnObjectState			m_objectState = enObjectState_Quiescence;	//オブジェクトのステート(状態)を表す変数
	float					m_degree = 0.0f;							//フワフワ用の角度
	float					m_damage = 0.0f;							//与えるダメージ
	float					m_collisionAddPos = 0.0f;					//コリジョンオブジェクトのY値加算量
	bool					m_canAttract = false;						//引き寄せれるかどうか

	nsPlayer::Player*		m_player = nullptr;
	Game*					m_game = nullptr;
	CollisionObject*		m_collisionObject = nullptr;
};

