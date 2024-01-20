#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "GameInformation.h"
#include "Boss.h"
#include "BlackFade.h"
#include "GameStart.h"
#include "MakeSound.h"

namespace
{
	//注視点から視点までのベクトル
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 150.0f, -300.0f };
	const Vector3 TO_CAMERA_START_POSITION = { 0.0f, 150.0f, 300.0f };

	//注視点から視点までのベクトル(近距離時)
	const Vector3 TO_CAMERA_POSITION_NEAR = { 0.0f,100.0f,-270.0f };

	//注視点のローカルポジション
	const Vector3 TARGET_LOCAL_POSITION = { 0.0f,180.0f,0.0f };

	//注視点のローカルポジション(近距離時)
	const Vector3 TARGET_LOCAL_POSITION_NEAR = { 60.0f,150.0f,0.0f };

	//カメラの最大移動速度
	const float MAX_CAMERA_SPEED = 5000.0f;

	//カメラの当たり判定の大きさ
	const float CAMERA_COLLISION_SCALE = 5.0f;

	//カメラの移動速度
	const float CAMERA_MOVE_POWER_X = 2.0f;
	const float CAMERA_MOVE_POWER_Y = 1.5f;

	//遠平面
	const float FAR_PLANE = 35000.0f;

	//ボスまでのベクトル
	const Vector3 TO_CAMERA_POSITION_BOSS = { 380.0f, -30.0f, 140.0f };

	//オープニング中のベクトル
	const Vector3 TO_CAMERA_POSITION_OP = { 0.0f, 0.0f, 100.0f };

	//オープニングのカメラスピード
	const float OP_CAMERA_SPEED = 4.0f;

	//オープニングの視点の位置
	const Vector3 OP_CAMERA_POS_1 = { -700.0f,150.0f,100.0f };
	const Vector3 OP_CAMERA_POS_2 = { 240.0f,150.0f,1700.0f };

	//フェードが始まるまでの時間
	const float FADE_START_TIME = 2.0f;

	//ボスを出現させるまでの時間
	const float MAKE_BOSS_TIME = 0.7f;

	//揺れの減少量
	const int VIB_DECREASE_AMOUNT = 10;

	//揺れの最大値
	const float VIBRATION_MAX = 640.0f;
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
	m_player = FindGO<nsPlayer::Player>("player");

	//インフォメーションのインスタンスを探す。
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//ボスのインスタンスを探す
	m_boss = FindGO<nsBoss::Boss>("boss");

	//ブラックフェードを探す
	m_blackFade = FindGO<BlackFade>("blackfade");

	//カメラまでの距離にセット
	m_toCameraPos.Set(TO_CAMERA_START_POSITION);

	//遠平面の設定
	g_camera3D->SetFar(FAR_PLANE);

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,			//ばねカメラの処理を行うカメラを指定する。
		MAX_CAMERA_SPEED,		//カメラの移動速度の最大値。
		true,					//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		CAMERA_COLLISION_SCALE	//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	//最初のターゲット
	m_opTarget = OP_CAMERA_POS_1;

	return true;
}

void GameCamera::OpeningProcess()
{
	//オープニング中じゃないなら返す
	if (m_gameInfo->GetIsInOpening() == false)
	{
		return;
	}

	//時間計測
	m_opTime += g_gameTime->GetFrameDeltaTime();

	//視点の設定
	Vector3 pos = Vector3::Zero;

	//場所①でのカメラ横移動
	if (m_opState == enOP_Camera1)
	{
		//注視点を移動させる
		m_opTarget.x += OP_CAMERA_SPEED;

		//フェードの処理
		CalcOpeningFade(enOP_Camera2, OP_CAMERA_POS_2);

		//視点を計算する。
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	//場所②でのカメラ移動
	else if (m_opState == enOP_Camera2)
	{
		//画面が明るくなったら
		if (m_blackFade->GetBlackAlpha() <= 0.0f && m_camera1End == false)
		{
			//フェードアウトの終了
			m_blackFade->SetAlphaDown(false);

			m_camera1End = true;
		}

		//注視点を移動させる
		m_opTarget.x -= OP_CAMERA_SPEED;
		
		//フェードの処理
		CalcOpeningFade(enOP_Player, CalcTargetPosition());

		//視点を計算する。
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	//プレイヤーの位置でのカメラ処理
	else if (m_opState == enOP_Player)
	{
		//画面が明るくなったら
		if (m_blackFade->GetBlackAlpha() <= 0.0f)
		{
			//フェードアウトの終了
			m_blackFade->SetAlphaDown(false);

			//オープニングの終了
			m_gameInfo->SetInOpening(false);

			//バトルスタートを表示(ゲームのスタートはこの中で行っている)
			NewGO<GameStart>(0,"gamestart");

			//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
			SoundSource* startSE = NewGO<SoundSource>(0);
			startSE->Init(enSoundName_OpDistortion);					//初期化
			startSE->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//音量調整
			startSE->Play(false);
		}

		//視点を計算する。
		pos = m_opTarget + TO_CAMERA_START_POSITION;
	}

	//視点と注視点の設定
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(m_opTarget);
}

void GameCamera::CalcOpeningFade(const enOPState& nextState, const Vector3& nextPos)
{
	if (m_opTime <= FADE_START_TIME)
	{
		return;
	}
		 
	//フェードインの開始
	m_blackFade->SetAlphaUp(true);

	//画面が真っ暗になったら
	if (m_blackFade->GetBlackAlpha() >= 1.0f)
	{
		//次のカメラ場所に移動
		m_opTarget = nextPos;
		m_springCamera.Refresh();

		//フェードインの終了
		m_blackFade->SetAlphaUp(false);

		//フェードアウトの開始
		m_blackFade->SetAlphaDown(true);

		//タイマーのリセット
		m_opTime = 0.0f;

		//次のステートへ移動
		m_opState = nextState;
	}
}

void GameCamera::UpdatePositionAndTarget()
{
	//ゲーム中じゃないなら返す
	if (m_gameInfo->GetIsInGame() == false)
	{
		return;
	}

	//遠近カメラの処理
	CalcChangeCamera();

	//注視点を求める
	Vector3 target = CalcTargetPosition();

	//揺れの大きさを足す
	target.x += BGX;
	target.y += BGY;

	//直前のベクトルを保存
	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力量
	float x = 0.0f, y = 0.0f;

	//遠近カメラが切り替わっている時じゃないなら
	if (m_isChangingCamera != true)
	{
		//パッドの入力を使ってカメラを回す
		x = g_pad[0]->GetRStickXF();
		y = g_pad[0]->GetRStickYF();
	}

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
	else if (toPosDir.y > 0.6f)
	{
		m_toCameraPos = toCameraPosOld;

		//遠近カメラ切り替わり中なら
		if (m_isChangingCamera)
		{
			//カメラの向きを平行に戻す
			qRot.AddRotationDegX(5.0f);
			qRot.Apply(m_toCameraPos);
		}
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

		//遠近カメラ切り替わり中にする
		m_isChangingCamera = true;
	}

	//カメラの回転に合わせたローカルポジションを計算
	Vector3 localPos = toCameraPos;
	rot.Apply3x3(localPos);

	//カメラまでの距離にセット
	m_toCameraPos.Set(localPos);
}

void GameCamera::CalcChangeCamera()
{
	//遠近カメラ切り替わり中なら
	if (m_isChangingCamera)
	{
		//時間計測
		m_cameraChangeTime -= g_gameTime->GetFrameDeltaTime();

		//一定時間経過で
		if (m_cameraChangeTime <= 0.0f)
		{
			//切り替わり終了
			m_isChangingCamera = false;

			//時間のリセット
			m_cameraChangeTime = 0.5f;
		}
	}
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

void GameCamera::BossMovieProcess()
{
	//ムービー中じゃないなら返す
	if (m_gameInfo->GetIsInMovie() == false)
	{
		return;
	}

	//注視点を求める
	Vector3 target = m_boss->GetPosition();
	target.y = 180.0f;
	target.z -= 20.0f;

	//揺れの大きさを足す
	target.x += BGX;
	target.y += BGY;

	//視点を計算する。
	Vector3 pos = target + TO_CAMERA_POSITION_BOSS;
	
	//バネカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
	
	//一定時間経過でボス出現
	m_bossMakeTime += g_gameTime->GetFrameDeltaTime();
	if (m_bossMakeTime >= MAKE_BOSS_TIME)
	{
		//ボス戦スタート
		m_gameInfo->StartBossBattle();
	}
}

void GameCamera::PlayVibration(int decline, int maxVib, bool& flag, int& vibration)
{
	//振幅の設定
	int vib = vibration / 2;

	//揺れを徐々に小さくする
	vibration -= decline;

	//揺れの大きさが0になったら
	if (vib == 0)
	{
		//揺れの為に必要な変数を初期化
		flag = false;
		BGX = 0; BGY = 0;
		vibration = maxVib;
	}
	else
	{
		//左右に揺れるために半分の値を引く
		int vibHalf = vib / 2;
		BGX = rand() % vib - vibHalf;
		BGY = rand() % vib - vibHalf;
	}
}

void GameCamera::ManageVibration()
{
	//揺れのフラグが立ったら
	if (m_vibFlag == true)
	{
		//揺れの最大値を設定
		static int Vibration = VIBRATION_MAX;

		//揺れの処理
		PlayVibration(VIB_DECREASE_AMOUNT, VIBRATION_MAX, m_vibFlag, Vibration);
	}
}

void GameCamera::Update()
{
	//オープニング中の処理
	OpeningProcess();

	//ゲーム中のカメラの視点と注視点の設定
	UpdatePositionAndTarget();

	//ボスムービー中の処理
	BossMovieProcess();

	//画面揺れの処理
	ManageVibration();

	//カメラの更新。
	m_springCamera.Update();
}

