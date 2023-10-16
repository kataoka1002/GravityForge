#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Move();
	void Turn();
	void ManageState();

	const Vector3& GetPosition() const
	{
		return m_position;
	}

public:
	enum EnPlayerState
	{
		enPlayerState_Idle,		//待機中
		enPlayerState_Walk,		//歩き中
		enPlayerState_Jump,		//ジャンプ中
		enPlayerState_Num
	};

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Num,
	};

	AnimationClip		animationClips[enAnimClip_Num];			//アニメーションクリップ
	ModelRender			m_playerModel;							//プレイヤーモデル
	Vector3				m_position = Vector3::Zero;				//座標
	Vector3				m_startPosition = Vector3::Zero;		//初期座標
	Vector3				m_moveSpeed = Vector3::Zero;			//移動速度
	CharacterController m_charaCon;								//キャラクターコントローラー
	Quaternion			m_rotation;								//クォータニオン
	EnPlayerState		m_playerState = enPlayerState_Idle;		//プレイヤーのステート(状態)を表す変数
};
