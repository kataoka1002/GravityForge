#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"

namespace
{
	
}

Player::Player()
{
	//アニメーションの初期化
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Walk].Load("Assets/animData/player/player_walk.tka");
	animationClips[enAnimClip_Walk].SetLoopFlag(true);
	animationClips[enAnimClip_Attract].Load("Assets/animData/player/player_attract.tka");
	animationClips[enAnimClip_Attract].SetLoopFlag(false);
	animationClips[enPlayerState_Standby].Load("Assets/animData/player/player_standby.tka");
	animationClips[enPlayerState_Standby].SetLoopFlag(true); 
	animationClips[enAnimClip_Standwalk].Load("Assets/animData/player/player_standwalk.tka");
	animationClips[enAnimClip_Standwalk].SetLoopFlag(true);
}

Player::~Player()
{

}

bool Player::Start()
{
	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetScale(3.0f);
	m_playerModel.SetPosition(Vector3{ 0.0f,50.0f,100.0f });
	m_playerModel.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		20.0f,			//半径
		120.0f,			//高さ
		m_position		//座標
	);

	return true;
}

void Player::Update()
{
	//移動の処理
	Move();

	//回転処理
	Turn();

	//アクションの処理
	Action();

	//ステート管理
	ManageState();

	//アニメーションの再生
	PlayAnimation();

	//更新
	m_playerModel.Update();
}

void Player::Move()
{
	//移動速度の初期化
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を受け取る
	float LStick_x = g_pad[0]->GetLStickXF();
	float LStick_y = g_pad[0]->GetLStickYF();

	//カメラの前方方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//XZ成分の移動速度をクリア
	m_moveSpeed += cameraForward * LStick_y * 500.0f;	//奥方向への移動速度を加算
	m_moveSpeed += cameraRight * LStick_x * 500.0f;		//右方向への移動速度を加算

	//キャラクターコントローラーを使用して座標を更新
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//座標を設定
	m_playerModel.SetPosition(m_position);
}

void Player::Turn()
{
	//このフレームではキャラは移動していないので旋回する必要はない
	if (fabsf(m_moveSpeed.x) < 0.001f && fabsf(m_moveSpeed.z) < 0.001f) 
	{
		return;
	}

	m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 8.0f, m_rotMove, m_moveSpeed);
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);


	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数
	//回転角度を求めている
	///float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//m_rotation.SetRotationY(-angle);

	//回転を設定する
	m_playerModel.SetRotation(m_rotation);
}

void Player::Action()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitAttract(m_position);

		m_playerState = enPlayerState_Attract;
	}
}

void Player::ManageState()
{
	//地面に付いていなかったら
	if (m_charaCon.IsOnGround() == false)
	{
		//ステートをジャンプ中にする
		m_playerState = enPlayerState_Jump;

		return;
	}

	//引き寄せ,スタンバイ,ステートなら
	if (m_playerState == enPlayerState_Attract)
	{
		//アニメーションの再生が終わったら。
		if (m_playerModel.IsPlayingAnimation() == false)
		{
			//スタンバイにする
			m_playerState = enPlayerState_Standby;
			m_isHoldingObject = true;
		}

		return;
	}

	//オブジェクトを持っていないとき
	if (m_isHoldingObject == false)
	{
		//地面に着いてxかzの移動速度があったら(スティックの入力があったら)
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//ステートを走りにする。
			m_playerState = enPlayerState_Walk;
		}
		//xとzの移動速度が無かったら(スティックの入力が無かったら)
		else
		{
			//ステートを待機にする。
			m_playerState = enPlayerState_Idle;
		}
	}
	//持っているとき
	else
	{
		//xかzの移動速度があったら(スティックの入力があったら)
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//ステートを構え歩きにする。
			m_playerState = enPlayerState_Standwalk;
		}
		else
		{
			//スタンバイにする
			m_playerState = enPlayerState_Standby;
		}
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//プレイヤーステートが待機だったら
	case enPlayerState_Idle:
		//待機アニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Idle, 0.2f);
		break;

	//プレイヤーステートが歩き中だったら
	case enPlayerState_Walk:
		//歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Walk, 0.2f);
		break;
		
	//プレイヤーステートが引き寄せだったら
	case enAnimClip_Attract:
		//引き寄せアニメーションを再生
		m_playerModel.PlayAnimation(enPlayerState_Attract, 0.5f);
		break;
		
	//プレイヤーステートがスタンバイだったら
	case enPlayerState_Standby:
		//引き寄せアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Standby, 0.5f);
		break; 
			
	//プレイヤーステートが構え歩きだったら
	case enPlayerState_Standwalk:
		//構え歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Standwalk, 0.2f);

	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}