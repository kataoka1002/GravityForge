#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	//注視点から視点までのベクトル
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 150.0f, -300.0f };

	//注視点から視点までのベクトル(近距離時)
	const Vector3 TO_CAMERA_POSITION_NEAR = { 0.0f,100.0f,-270.0f };

	//注視点のローカルポジション
	const Vector3 TARGET_LOCAL_POSITION = { 0.0f,180.0f,0.0f };

	//注視点のローカルポジション(近距離時)
	const Vector3 TARGET_LOCAL_POSITION_NEAR = { 60.0f,150.0f,0.0f };

	//カメラの最大移動速度
	const float MAX_CAMERA_SPEED = 800.0f;

	//カメラの当たり判定の大きさ
	const float CAMERA_COLLISION_SCALE = 5.0f;

	//カメラの移動速度
	const float CAMERA_MOVE_POWER_X = 1.0f;
	const float CAMERA_MOVE_POWER_Y = 1.0f;

	//遠平面
	const float FAR_PLANE = 15000.0f;
}

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//注視点から視点までのベクトルを求める
	SetNearCamera(false);

	//遠平面の設定
	g_camera3D->SetFar(FAR_PLANE);

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,			//ばねカメラの処理を行うカメラを指定する。
		MAX_CAMERA_SPEED,		//カメラの移動速度の最大値。
		true,					//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		CAMERA_COLLISION_SCALE	//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	return true;
}

void GameCamera::UpdatePositionAndTarget()
{
	//注視点を求める
	Vector3 target = CalcTargetPosition();

	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, x * CAMERA_MOVE_POWER_X);
	qRot.Apply(m_toCameraPos);

	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, y * CAMERA_MOVE_POWER_Y);
	qRot.Apply(m_toCameraPos);

	//カメラの回転の上限をチェックする。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	//カメラが上向きすぎ
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	//カメラが下向きすぎ
	else if (toPosDir.y > 0.8f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//バネカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::SetNearCamera(bool isNear)
{
	//カメラの回転を渡す
	Matrix rot = m_springCamera.GetCameraRotation();

	Vector3 toCameraPos;

	//プレイヤーがスタンバイ中なら
	if (isNear == true)
	{
		//近距離カメラON
		m_isNearCamera = true;

		//注視点から視点までの距離を設定
		toCameraPos = TO_CAMERA_POSITION_NEAR;

		//カメラの回転行列の修正
		rot.m[0][1] = 1.0f;
		rot.m[1][1] = 0.0f;
		rot.m[2][1] = 0.0f;
		rot.m[3][1] = 0.0f;
	}
	else
	{
		//近距離カメラOFF
		m_isNearCamera = false;

		//注視点から視点までの距離を設定
		toCameraPos = TO_CAMERA_POSITION;
	}

	//カメラの回転に合わせたローカルポジションを計算
	Vector3 localPos = toCameraPos;
	rot.Apply3x3(localPos);

	//カメラまでの距離にセット
	m_toCameraPos.Set(localPos);
}

Vector3 GameCamera::CalcTargetPosition()
{
	//注視点を計算する。
	Vector3 target = m_player->GetPosition();

	//近距離カメラのときなら
	if (m_isNearCamera)
	{
		//プレイヤーの回転を渡す
		Quaternion rot = m_player->GetRotation();

		//プレイヤーの回転に合わせたローカルポジションを計算
		Vector3 localPos = TARGET_LOCAL_POSITION_NEAR;
		rot.Multiply(localPos);

		//プレイヤーのポジションに計算したローカルポジションを足す
		target += localPos;
	}
	else
	{
		//プレイヤーの足元からちょっと上を注視点とする。
		target += TARGET_LOCAL_POSITION;
	}

	//少し前を注視点にする
	target += g_camera3D->GetForward() * 20.0f;	

	return target;
}

void GameCamera::Update()
{
	//カメラの視点と注視点の設定
	UpdatePositionAndTarget();

	//カメラの更新。
	m_springCamera.Update();
}

