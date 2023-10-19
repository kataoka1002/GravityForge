#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include <math.h>

namespace
{
	//π
	const float PI = 3.141592;

	//引き寄せられる速さ
	const float OBJECT_SPPED = 20.0f;
	const float OBJECT_VELOCITY = 10.0f;

	//カーブの角度
	const float CURVATURE_RADIUS = 10.0f;
	const float DAMPING = 0.1f;

	//上下にフワフワする速さ
	const float UP_DOWN_SPEED = 1.5f;

	//オブジェクトのローカルポジション
	const Vector3 OBJECT_LOCAL_POSITION = { 100.0f,150.0f,0.0f };

	//吹っ飛ぶ速さ
	const float BLOW_AWAY_SPEED = 3000.0f;
}

bool ObjectBase::Start()
{
	//プレイヤーを探す
	m_player = FindGO<Player>("player");

	//モデルの初期化
	InitModel();

	return true;
}

void ObjectBase::Move()
{
	switch (m_objectState)
	{
	//静止中
	case enObjectState_Quiescence:
		break;

	//待機中
	case enObjectState_Idle:
		//フワフワ
		IdleMove();

		//回転
		Turn(g_camera3D->GetForward());

		break;

	//引き寄せ中
	case enObjectState_Attract:
		//ターゲットの設定
		CalcTargetPosition();

		//引き寄せる
		AttractedToPlayer();

		//回転
		Turn(g_camera3D->GetForward());

		break;

	//吹っ飛び中
	case enObjectState_Blow:
		//吹っ飛ぶ処理
		BlowAway();

		break;

	default:
		break;
	}

	//更新
	m_charaCon.SetPosition(m_position);
	m_model.SetPosition(m_position);
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

	//プレイヤーのポジションにローカルポジションを足した場所をターゲットにする
	Vector3 target = m_player->GetPosition() + localPos;

	//滑らかに移動させる
	m_position = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_position, target);
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
	//ターゲットまでの方向、距離を計算
	Vector3 toTarget = m_targetPosition - m_position;
	Vector3 toTargetDir = toTarget;
	toTargetDir.Normalize();

	//ターゲットに少し近づいたら
	if (toTarget.Length() <= 200.0f)
	{
		//移動量を計算する
		Vector3 speed = toTargetDir * 10.0f;
		m_position += speed;
	}
	else
	{
		//移動量を計算する
		CalcVelocity(OBJECT_SPPED, CURVATURE_RADIUS, DAMPING);
		m_position += m_velocity;
	}

	//ターゲットに近づいたら
	if (toTarget.Length() <= 5.0f)
	{
		//待機中のステートに変更
		m_objectState = enObjectState_Idle;
	}
}

void ObjectBase::InitAttract()
{
	//ターゲットの設定
	CalcTargetPosition();

	//ターゲットまでの距離,方向を計算
	Vector3 toTarget = m_targetPosition - m_position;
	Vector3 toTargetDir = toTarget;
	toTargetDir.Normalize();

	//飛び出す方向をランダムで決める
	Quaternion rotation;
	rotation.SetRotationDeg(Vector3{0.0f,0.0f,1.0f}, rand() % 180);

	//右方向の設定
	Vector3 m_right = Vector3::AxisX;

	//クォータニオンによる回転が適用された新しいベクトルが得られる
	rotation.Apply(m_right);

	//初速
	SetVelocity(m_right * OBJECT_VELOCITY);

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

void ObjectBase::CalcTargetPosition()
{
	//プレイヤーの回転を渡す
	Quaternion rot = m_player->GetRotation();

	//プレイヤーの回転に合わせたローカルポジションを計算
	Vector3 localPos = OBJECT_LOCAL_POSITION;
	rot.Multiply(localPos);

	//プレイヤーのポジションを渡す	
	m_targetPosition = m_player->GetPosition();

	//プレイヤーのポジションに計算したローカルポジションを足す
	m_targetPosition += localPos;
}

void ObjectBase::Turn(Vector3 speed)
{
	//滑らかに回転させる
	m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, speed);
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);

	m_model.SetRotation(m_rotation);
}

void ObjectBase::InitBlowAway()
{
	//飛んでいく方向の決定(カメラが向いている方向に飛んでいく)
	m_flightSpeed = g_camera3D->GetForward() * BLOW_AWAY_SPEED;

	//吹っ飛びステートに変更
	m_objectState = enObjectState_Blow;
}

void ObjectBase::BlowAway()
{	
	//吹っ飛び中のキャラコンの更新
	m_position = m_charaCon.Execute(m_flightSpeed, g_gameTime->GetFrameDeltaTime());
}