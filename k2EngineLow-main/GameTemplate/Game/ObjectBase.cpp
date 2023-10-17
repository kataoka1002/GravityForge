#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include <math.h>

namespace
{
	const float PI = 3.141592;
	const float OBJECT_SPPED = 20.0f;
	const float OBJECT_VELOCITY = 10.0f;
	const float CURVATURE_RADIUS = 30.0f;
	const float DAMPING = 0.1f;

	const float UP_DOWN_SPEED = 15.0f;
	const Vector3 OBJECT_LOCAL_POSITION = { 50.0f,200.0f,0.0f };
}

bool ObjectBase::Start()
{
	//プレイヤーを探す
	m_player = FindGO<Player>("player");

	//モデルの初期化
	InitModel();

	return true;
}

void ObjectBase::ManageState()
{
	//初期の静止した状態の時
	if (m_objectState == enObjectState_Quiescence)
	{
		return;
	}
	//引き寄せ中
	if (m_objectState == enObjectState_Attract)
	{
		AttractedToPlayer();
	}
	//待機中
	if (m_objectState == enObjectState_Idle)
	{
		IdleMove();
	}
}

void ObjectBase::IdleMove()
{
	//プレイヤーを追従する
	FollowPlayer();

	//上下に動く
	UpDown();
}

void ObjectBase::FollowPlayer()
{
	//プレイヤーの回転を渡す
	m_rotation = m_player->GetRotation();

	//プレイヤーの回転に合わせたローカルポジションを計算
	Vector3 localPos = OBJECT_LOCAL_POSITION;
	m_rotation.Multiply(localPos);

	//プレイヤーのポジションを渡す	
	m_position = m_player->GetPosition();

	//プレイヤーのポジションに計算したローカルポジションを足す
	m_position += localPos;
}

void ObjectBase::UpDown()
{
	//sin波を使って滑らかに上下させる
	//角度を大きくしていく
	degree += 1.0f;

	//360度で-1～1を一周するので0度に戻す
	if (degree >= 360.0f)
	{
		degree = 0.0f;
	}

	//角度をラジアンに変換
	float rad = degree * PI / 180.0f;

	//sinの値を求める
	float sinValue = sin(rad);

	//求めた値を速さにかけて最終的な速さを求める
	float UpDownSpeed = UP_DOWN_SPEED * sinValue;
	m_position.y += UpDownSpeed;
}

void ObjectBase::AttractedToPlayer()
{
	//移動量を計算する
	CalcVelocity(OBJECT_SPPED, CURVATURE_RADIUS, DAMPING);
	m_position += m_velocity;

	//ターゲットまでの距離を計算
	Vector3 toTarget = m_targetPosition - m_position;

	//ターゲットに近づいたら
	if (toTarget.Length() <= 10.0f)
	{
		//待機中のステートに変更
		m_objectState = enObjectState_Idle;
	}
}

void ObjectBase::InitAttract(Vector3 targetPos)
{
	//飛び出す方向をランダムで決める
	Quaternion rotation;
	rotation.SetRotationDeg(Vector3{ 0.0f,0.0f,1.0f }, rand() % 180);

	//右方向の設定
	Vector3 m_right = Vector3::AxisX;

	//クォータニオンによる回転が適用された新しいベクトルが得られる
	rotation.Apply(m_right);

	//初速
	SetVelocity(m_right * OBJECT_VELOCITY);

	//ターゲットを頭の上にする
	targetPos.y += 200.0f;

	//ターゲットの設定
	SetTargetPos(targetPos);

	//引き寄せステートに変更
	m_objectState = enObjectState_Attract;
}

void ObjectBase::CalcVelocity(const float speed, const float curvatureRadius, const float damping)
{
	//遠心力を求める
	float maxCentripetalAccel = speed * speed / curvatureRadius;

	//スピードに減衰率をかけて逆方向への力を求める
	float propulsion = speed * damping;

	//目標の地点
	Vector3 targetPosition = m_targetPosition;

	//目標までのベクトル
	Vector3 toTarget = targetPosition - m_position;

	//速度を設定し、正規化して打ち出された方向を取得
	Vector3 vn = m_velocity;
	vn.Normalize();

	//目標までのベクトルと打ち出された方向の内積を求める(敵へ向かうベクトルをvnに射影し T'の大きさが求まる)
	float dot = toTarget.Dot(vn);

	//T'から目標までのベクトル(曲げるために加える加速度)を求める
	Vector3 centripetalAccel = toTarget - (vn * dot);

	//その長さを求める
	float centripetalAccelMagnitude = centripetalAccel.Length();

	//1以上なら長さを1に正規化し,1未満ならそのまま
	if (centripetalAccelMagnitude > 1.0f)
	{
		//ベクトルの正規化を行う
		centripetalAccel /= centripetalAccelMagnitude;
	}

	//長さがmaxCentripetalAccel以下ならそのまま、以上なら長さをmaxCentripetalAccelにする
	Vector3 force = centripetalAccel * maxCentripetalAccel;

	//推進力
	force += vn * propulsion;

	//空気抵抗
	force -= m_velocity * damping;

	//速度積分
	m_velocity += force * g_gameTime->GetFrameDeltaTime();
}