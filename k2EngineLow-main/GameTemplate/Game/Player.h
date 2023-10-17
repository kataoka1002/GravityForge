#pragma once
class Teapot;

class Player : public IGameObject
{
public:
	enum EnPlayerState
	{
		enPlayerState_Idle,		//待機中
		enPlayerState_Walk,		//歩き中
		enPlayerState_Jump,		//ジャンプ中
		enPlayerState_Attract,	//引き寄せ中
		enPlayerState_Standby,	//待機中(オブジェクトを持っている時)
		enPlayerState_Standwalk,//構えて歩く
		enPlayerState_Attack,	//攻撃
		enPlayerState_Num
	};

public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Turn();
	void Action();
	void PlayAnimation();
	void ManageState();

	void SetStandbyState()
	{
		//スタンバイステートにする
		m_playerState = enPlayerState_Standby;
	}

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	const EnPlayerState& GetPlayerState()const
	{
		return m_playerState;
	}

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Jump,
		enAnimClip_Attract,
		enAnimClip_Standby,
		enAnimClip_Standwalk,
		enAnimClip_Attack,	
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];			//アニメーションクリップ
	ModelRender			m_playerModel;							//プレイヤーモデル
	Vector3				m_position = Vector3::Zero;				//座標
	Vector3				m_startPosition = Vector3::Zero;		//初期座標
	Vector3				m_moveSpeed = Vector3::Zero;			//移動速度
	CharacterController m_charaCon;								//キャラクターコントローラー
	Quaternion			m_rotation;								//クォータニオン
	EnPlayerState		m_playerState = enPlayerState_Idle;		//プレイヤーのステート(状態)を表す変数
	bool				m_isHoldingObject = false;				//オブジェクトを持っているかどうか
	Vector3             m_rotMove = Vector3::Zero;

	Teapot* m_teapot = nullptr;
};
