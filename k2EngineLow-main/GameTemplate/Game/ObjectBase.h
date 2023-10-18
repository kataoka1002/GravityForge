#pragma once
class Player;

class ObjectBase : public IGameObject, Noncopyable
{
public:

	//コンストラクタ
	ObjectBase() {}
	//デストラクタはバーチャルにしておく
	virtual ~ObjectBase() {}

	bool Start();
	void AttractedToPlayer();						//プレイヤーまで引き寄せられる処理
	void CalcVelocity(const float speed, const float curvatureRadius, const float damping);
	void InitAttract();
	void Move();
	void IdleMove();
	void FollowPlayer();
	void UpDown();
	void Turn(Vector3 speed);
	void CalcTargetPosition();
	void InitBlowAway();
	void BlowAway();

	virtual void Render(RenderContext& rc)	= 0;	//描画処理
	virtual void InitModel()				= 0;	//モデルの初期化

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
	Vector3					m_position = Vector3::Zero;					//ポジション
	Vector3					m_targetPosition = Vector3::Zero;			//目的地
	Vector3					m_velocity = Vector3::Zero;					//速度
	Vector3					m_rotMove = Vector3::Zero;
	Vector3					m_flightSpeed = Vector3::Zero;
	Quaternion				m_rotation;									//クォータニオン
	EnObjectState			m_objectState = enObjectState_Quiescence;	//オブジェクトのステート(状態)を表す変数
	float degree = 0.0f;

	Vector3 m_followSpeed = Vector3::Zero;

	Player* m_player = nullptr;
};

