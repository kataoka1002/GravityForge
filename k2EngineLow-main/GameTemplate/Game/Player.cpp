#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"
#include "GameCamera.h"

namespace
{
	const float MOVE_SPEED = 250.0f;
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
	animationClips[enAnimClip_Attack].Load("Assets/animData/player/player_attack.tka");
	animationClips[enAnimClip_Attack].SetLoopFlag(false);
	animationClips[enAnimClip_WalkRight].Load("Assets/animData/player/player_walk_right.tka");
	animationClips[enAnimClip_WalkRight].SetLoopFlag(true);
	animationClips[enAnimClip_WalkLeft].Load("Assets/animData/player/player_walk_left.tka");
	animationClips[enAnimClip_WalkLeft].SetLoopFlag(true);
	animationClips[enAnimClip_WalkBack].Load("Assets/animData/player/player_walk_back.tka");
	animationClips[enAnimClip_WalkBack].SetLoopFlag(true);

}

Player::~Player()
{

}

bool Player::Start()
{
	m_camera = FindGO<GameCamera>("gamecamera");

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
	//引き寄せ中,攻撃中は動けない
	if (m_playerState == enPlayerState_Attract || m_playerState == enPlayerState_Attack)
	{
		return;
	}

	//移動速度の初期化
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を受け取る
	LStick_x = g_pad[0]->GetLStickXF();
	LStick_y = g_pad[0]->GetLStickYF();

	//カメラの前方方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//XZ成分の移動速度をクリア
	m_moveSpeed += cameraForward * LStick_y * MOVE_SPEED;	//奥方向への移動速度を加算
	m_moveSpeed += cameraRight * LStick_x * MOVE_SPEED;		//右方向への移動速度を加算

	//キャラクターコントローラーを使用して座標を更新
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//座標を設定
	m_playerModel.SetPosition(m_position);
}

void Player::Turn()
{
	//攻撃中は回れない
	if (m_playerState == enPlayerState_Attack)
	{
		return;
	}

	//滑らかに回るようにする
	//オブジェクトを持っている時
	if (m_playerState == enPlayerState_Standby || m_playerState == enPlayerState_Standwalk
		|| m_playerState == enPlayerState_Attract || m_playerState == enPlayerState_WalkLeft 
		|| m_playerState == enPlayerState_WalkRight || m_playerState == enPlayerState_WalkBack)
	{	
		//カメラの向きから回転を求める
		Vector3 rotSpeed = g_camera3D->GetForward();
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, rotSpeed);
	}
	else
	{
		//移動速度から回転を求める
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 8.0f, m_rotMove, m_moveSpeed);
	}

	//回転を設定する
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	m_playerModel.SetRotation(m_rotation);
}

void Player::Action()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitAttract();

		m_playerState = enPlayerState_Attract;

		//カメラを近距離にする
		m_camera->SetNearCamera(true);
	}

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_playerState = enPlayerState_Attack;

		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitBlowAway();
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

	//引き寄せステートなら
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

	//攻撃ステートなら
	if (m_playerState == enPlayerState_Attack)
	{
		//アニメーションの再生が終わったら。
		if (m_playerModel.IsPlayingAnimation() == false)
		{
			//アイドルにする
			m_playerState = enPlayerState_Idle;
			m_isHoldingObject = false;

			//カメラを遠距離に戻す
			m_camera->SetNearCamera(false);
		}

		return;
	}

	//オブジェクトを持っているとき
	if (m_isHoldingObject) 
	{
		//スティックの入力量によってステートを変更
		if (LStick_x <= -0.5f)
		{
			//左歩きステートにする
			m_playerState = enPlayerState_WalkLeft;
			return;
		}
		else if (LStick_x >= 0.5f)
		{
			//右歩きステートにする
			m_playerState = enPlayerState_WalkRight;
			return;
		}

		if (LStick_y <= -0.5f)
		{
			//後ろ歩きステートにする
			m_playerState = enPlayerState_WalkBack;
			return;
		}
		
		m_playerState = (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) ? enPlayerState_Standwalk : enPlayerState_Standby;
	}
	//持っていないとき
	else 
	{
		//スティックの入力量によってステートを変更
		m_playerState = (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) ? enPlayerState_Walk : enPlayerState_Idle;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//プレイヤーステートが待機だったら
	case enPlayerState_Idle:
		//待機アニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Idle, 0.5f);
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
		break;

	//プレイヤーステートが構え歩きだったら
	case enPlayerState_Attack:
		//構え歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Attack, 0.2f);
		break;

	//プレイヤーステートが右歩きだったら
	case enPlayerState_WalkRight:
		//右歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_WalkRight, 0.3f);
		break;

	//プレイヤーステートが左歩きだったら
	case enPlayerState_WalkLeft:
		//左歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_WalkLeft, 0.3f);
		break;

	//プレイヤーステートが後ろ歩きだったら
	case enPlayerState_WalkBack:
		//後ろ歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_WalkBack, 0.3f);
		break;

	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}

