#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include "Game.h"
#include <math.h>
#include "collision/CollisionObject.h"

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

	//コリジョンの大きさ
	const float COLLISION_SCALE = 30.0f;
}

bool ObjectBase::Start()
{
	//プレイヤーを探す
	m_player = FindGO<nsPlayer::Player>("player");

	//ゲームを探す
	m_game = FindGO<Game>("game");

	//モデルの初期化
	InitModel();

	m_sphereCollider.Create(1.0f);

	return true;
}

void ObjectBase::Move()
{
	switch (m_objectState)
	{
	//静止中
	case enObjectState_Quiescence:
		//引き寄せれるかどうかを判定
		CalcAimingDirection();
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

		CalcCollision();
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
	m_degree += 1.0f;

	//360度で-1～1を一周するので0度に戻す
	if (m_degree >= 360.0f)
	{
		m_degree = 0.0f;
	}

	//角度をラジアンに変換
	float rad = m_degree * PI / 180.0f;

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
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(
		m_position,				//座標
		Quaternion::Identity,	//回転
		COLLISION_SCALE			//半径
	);

	//コリジョンに名前を付ける
	m_collisionObject->SetName("teapot");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);

	//飛んでいく方向の決定(レティクルの方向)
	m_flightSpeed = g_camera3D->GetForward() * BLOW_AWAY_SPEED;
	m_flightSpeed += g_camera3D->GetRight() * -90.0f;

	//吹っ飛びステートに変更
	m_objectState = enObjectState_Blow;

	m_canAttract = false;
}

void ObjectBase::BlowAway()
{	
	//吹っ飛び中のキャラコンの更新
	m_position = m_charaCon.Execute(m_flightSpeed, g_gameTime->GetFrameDeltaTime());

	//コリジョンのポジションのセット
	m_collisionObject->SetPosition(m_position);

	//ぶつかったかどうかの処理
	CalcCollision();
}

void ObjectBase::CalcCollision()
{	
	//エネミーとぶつかったかを判定
	for (auto enemy : m_game->GetEnemyList())
	{
		if (m_collisionObject->IsHit(enemy->GetCharaCon()))
		{
			//ダメージを受けた時の処理を行う
			enemy->HandleDamageEvent(1.0f);

			//自分自身の削除
			DeleteGO(this);

			return;
		}
	}
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	//衝突フラグ。
	bool isHit = false;			

	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったらフラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};

void ObjectBase::CalcAimingDirection()
{
	//デフォルトは引き寄せれない
	m_canAttract = false;

	//カメラまでの方向を求める
	Vector3 toCameraDir = g_camera3D->GetPosition() - m_position;
	toCameraDir.Normalize();

	//ベクトルを逆向きにする
	toCameraDir *= -1.0f;

	//内積を求める
	float innerProduct = toCameraDir.Dot(g_camera3D->GetForward());

	//内積が１より小さいなら(2つのベクトルが別の方向を向いている)なら
	if (innerProduct < 0.9999f)
	{
		return;
	}

	//コライダーの始点と終点
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//始点はオブジェクトの座標
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//終点はカメラの座標
	end.setOrigin(btVector3(g_camera3D->GetPosition().x, g_camera3D->GetPosition().y, g_camera3D->GetPosition().z));

	//コライダーを始点から終点まで動かして,衝突するかどうかを調べる
	SweepResultWall callback;
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	//壁と衝突したら
	if (callback.isHit == true)
	{
		return;
	}

	//引き寄せ可能にする
	m_canAttract = true;
}