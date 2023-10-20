#pragma once
class Teapot;
class GameCamera;

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
		enPlayerState_WalkRight,//右に歩く
		enPlayerState_WalkLeft,	//左に歩く
		enPlayerState_WalkBack,	//後ろに歩く
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

	const Vector3& GetPlayerSpeed()const
	{
		return m_moveSpeed;
	}

	const bool& GetIsHoldingObject()const
	{
		return m_isHoldingObject;
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
		enAnimClip_WalkRight,
		enAnimClip_WalkLeft,
		enAnimClip_WalkBack,
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];			//アニメーションクリップ
	ModelRender			m_playerModel;							//プレイヤーモデル
	Vector3				m_position = Vector3::Zero;				//座標
	Vector3				m_startPosition = Vector3::Zero;		//初期座標
	Vector3				m_moveSpeed = Vector3::Zero;			//移動速度
	Vector3             m_rotMove = Vector3::Zero;				//回転速度
	CharacterController m_charaCon;								//キャラクターコントローラー
	Quaternion			m_rotation;								//クォータニオン
	EnPlayerState		m_playerState = enPlayerState_Idle;		//プレイヤーのステート(状態)を表す変数
	bool				m_isHoldingObject = false;				//オブジェクトを持っているかどうか
	float				LStick_x = 0.0f;						//Lスティック入力量(横)
	float				LStick_y = 0.0f;						//Lスティック入力量(縦)

	Teapot*				m_teapot = nullptr;
	GameCamera*			m_camera = nullptr;
};
