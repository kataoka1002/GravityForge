#include "stdafx.h"
#include "Player.h"

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
		100.0f,			//高さ
		m_position		//座標
	);

	return true;
}

void Player::Update()
{
	//移動の処理
	Move();

	//アニメーションの再生
	PlayAnimation();

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

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//プレイヤーステートが0(待機)だったら
	case enPlayerState_Idle:
		//待機アニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Idle);
		break;

	//プレイヤーステートが歩き中だったら
	case enPlayerState_Walk:
		//歩きアニメーションを再生
		m_playerModel.PlayAnimation(enAnimClip_Walk);
		break;

	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}