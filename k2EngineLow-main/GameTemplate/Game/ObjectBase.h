#pragma once
#include "TargetUI.h"
#include "MakeEffect.h"

// 前方宣言。
namespace nsPlayer { class Player; }
namespace nsBoss { class Boss; }
class Game;

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

	/// <summary>
	/// 落下中の処理
	/// </summary>
	void FallingObject();

	/// <summary>
	/// 地面に落下したかどうかの判定
	/// </summary>
	void CheckToLand();

	/// <summary>
	/// プレイヤーから見えているかチェック
	/// </summary>
	/// <returns>true : 見える , false : 見えない</returns>
	bool CheckCanSee();

	/// <summary>
	/// 飛び始めてからの時間を計算
	/// </summary>
	void CheckFlightTime();

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
	/// ステートを落下中にする
	/// </summary>
	void SetFallingState()
	{
		m_objectState = enObjectState_Falling;
	}

	/// <summary>
	/// 引き寄せれるかどうかの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetCanAttract() const
	{
		return m_canAttract;
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

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="pos">ポジション</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">大きさ</param>
	void PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// 爆発の処理
	/// </summary>
	void BombProcess();

protected:
	enum EnObjectState
	{
		enObjectState_Quiescence,	//初期状態
		enObjectState_Idle,			//待機中
		enObjectState_Attract,		//引き寄せ中
		enObjectState_Blow,			//吹っ飛び中
		enObjectState_Falling,		//落下中
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
	Vector3					m_flightDir = Vector3::Zero;				//飛んでいく方向
	Vector3					m_crossPosition = Vector3::Zero;			//レイとの交点の座標
	Vector3					m_localPosition = Vector3::Zero;			//持っている時のローカル座標
	Quaternion				m_rotation;									//クォータニオン
	EnObjectState			m_objectState = enObjectState_Quiescence;	//オブジェクトのステート(状態)を表す変数
	float					m_degree = 0.0f;							//フワフワ用の角度
	float					m_damage = 0.0f;							//与えるダメージ
	float					m_collisionAddPos = 0.0f;					//コリジョンオブジェクトのY値加算量
	float					m_diagonalRot = 0.0f;						//斜めの角度
	float					m_fallingTime = 0.0f;						//落下し始めてからの時間
	float					m_flightTime = 0.0f;						//飛び始めてからの時間
	bool					m_canAttract = false;						//引き寄せれるかどうか
	bool					m_makeTargetUI = false;						//targetUIを作ったかどうか
	bool					m_imSolarPanel = false;						//ソーラーパネルかどうか
	bool					m_canSeeModel = true;						//モデルを描画するかどうか
	int						m_smokeEfeInterval = 0;						//煙エフェクトが発生する間隔
	int						m_sandCount = 0;
	int						m_instanceNo = 0;							// インスタンス番号。

	nsPlayer::Player*		m_player = nullptr;
	nsBoss::Boss*			m_boss = nullptr;
	Game*					m_game = nullptr;
	CollisionObject*		m_collisionObject = nullptr;
	CollisionObject*		m_bombCollisionObject = nullptr;
	TargetUI*				m_targetUI = nullptr;
};

