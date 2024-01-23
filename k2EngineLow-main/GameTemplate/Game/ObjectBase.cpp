#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include "Game.h"
#include <math.h>
#include "collision/CollisionObject.h"
#include "Boss.h"
#include "MakeSound.h"

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
	const float BLOW_AWAY_SPEED = 4000.0f;

	//落下速度
	const float FALL_SPEED = 100.0f;

	//引き寄せれる距離
	const float ATTRACT_LIMIT = 800.0f;

	//描画する距離
	const float DRAW_LIMIT = 7000.0f;

	//斜めの角度の最大値
	const float MAX_DIAGONAL_ROTATION = 20.0f;

	//斜めになる速さ
	const float DIAGONAL_ROTATION_SPEED = 0.5f;

	//オブジェクトを落としたときのエフェクトの大きさ
	const Vector3 DROP_EFFECT_SCALE = { 1.9f,1.9f ,1.9f };

	//オブジェクトがステージにぶつかった時のエフェクトの大きさ
	const Vector3 HIT_STAGE_EFFECT_SCALE = { 2.9f,2.9f ,2.9f };

	//オブジェクトが飛び出して爆発するまでの時間
	const float BOMB_TIME = 2.5f;
}

bool ObjectBase::Start()
{
	//プレイヤーを探す
	m_player = FindGO<nsPlayer::Player>("player");

	m_boss = FindGO<nsBoss::Boss>("boss");

	//ゲームを探す
	m_game = FindGO<Game>("game");

	//ローカルポジションの設定
	m_localPosition = OBJECT_LOCAL_POSITION;

	//モデルの初期化
	InitModel();

	//壁に隠れているかを判定するためのコライダ
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
		//衝突したかどうかの処理
		CalcCollision();
		//時間測定
		CheckFlightTime();
		break;

		//落下中
	case enObjectState_Falling:
		//落下中の処理
		FallingObject();
		//着地判定
		CheckToLand();
		break;

	default:
		break;
	}

	//オブジェクトが静止状態じゃなくなったら
	if (m_objectState != enObjectState_Quiescence && m_targetUI != nullptr)
	{
		//UIの削除
		DeleteGO(m_targetUI);
	}

	//更新
	m_charaCon.SetPosition(m_position);
	//m_model.SetPosition(m_position);
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
	Vector3 localPos = m_localPosition;
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
	//最初の20フレームだけ
	if (m_sandCount <= 20)
	{
		if(m_sandCount % 2 == 0)
		{
			//持ち上げた時にエフェクトを出す
			PlayEffect(enEffectName_ObjectSand, m_position, m_rotation, Vector3::One);
		}

		m_sandCount++;
	}

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
	//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
	SoundSource* windSE = NewGO<SoundSource>(0);
	windSE->Init(enSoundName_ObjUpWind);						//初期化
	windSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
	windSE->Play(false);

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
	Vector3 localPos = m_localPosition;
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

	//斜めにする
	m_diagonalRot += DIAGONAL_ROTATION_SPEED;
	if (m_diagonalRot >= MAX_DIAGONAL_ROTATION)
	{
		m_diagonalRot = MAX_DIAGONAL_ROTATION;
	}
	m_rotation.SetRotationDegZ(m_diagonalRot);

	m_model.SetRotation(m_rotation);
}

void ObjectBase::InitBlowAway()
{
	//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
	SoundSource* windSE = NewGO<SoundSource>(0);
	windSE->Init(enSoundName_ObjBlowWind);					//初期化
	windSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
	windSE->Play(false);

	//エフェクト発生
	PlayEffect(enEffectName_ObjectPush, m_position, m_player->GetRotation(), Vector3::One);

	//コリジョンの初期化
	InitCollision();

	//飛んでいく方向の決定(レティクルの方向)
	m_flightDir = g_camera3D->GetForward();
	m_flightSpeed = g_camera3D->GetForward() * BLOW_AWAY_SPEED;
	m_flightSpeed += g_camera3D->GetRight() * -90.0f;

	//レイの始点と終点を決めてステージとの交点を求める
	Vector3 start, end;
	start = m_position;
	end = m_position += m_flightDir * 7000.0f;
	PhysicsWorld::GetInstance()->RayTest(start, end, m_crossPosition);

	//吹っ飛びステートに変更
	m_objectState = enObjectState_Blow;

	m_canAttract = false;
}

void ObjectBase::BlowAway()
{	
	//吹っ飛び中のキャラコンの更新
	m_position = m_charaCon.Execute(m_flightSpeed, g_gameTime->GetFrameDeltaTime());
	
	//コリジョンをキャラコンと同じ位置に出るようにセット
	m_collisionPosition = { m_position.x,m_position.y + m_collisionAddPos,m_position.z };

	//コリジョンのポジションのセット
	m_collisionObject->SetPosition(m_collisionPosition);

	//フレーム間隔でエフェクト発生
	m_smokeEfeInterval++;
	if (m_smokeEfeInterval >= 1)
	{
		//エフェクト発生
		PlayEffect(enEffectName_ObjectSmoke, m_position, m_player->GetRotation(), Vector3::One);

		m_smokeEfeInterval = 0;
	}
}

void ObjectBase::CheckFlightTime()
{
	//飛び始めてからの時間計測
	m_flightTime += g_gameTime->GetFrameDeltaTime();

	//一定時間経過で消える
	if (m_flightTime >= BOMB_TIME)
	{
		//エフェクト発生
		PlayEffect(enEffectName_ObjectDrop, m_position, m_rotation, DROP_EFFECT_SCALE);

		//自分が消えるときの処理
		OnDestroy();
	}
}

void ObjectBase::FallingObject()
{
	//落下速度の設定
	Vector3 fallSpeed = { 0.0f,0.0f,0.0f };

	//落ち始めてからの時間を計測する
	m_fallingTime += g_gameTime->GetFrameDeltaTime();

	//時間による自由落下の速さを計算
	fallSpeed.y = -9.8f * m_fallingTime * FALL_SPEED;
	
	//落下中のキャラコンの更新
	m_position = m_charaCon.Execute(fallSpeed, g_gameTime->GetFrameDeltaTime());
}

void ObjectBase::CheckToLand()
{
	//地面に着いたら
	if (m_charaCon.IsOnGround())
	{
		//エフェクト発生
		PlayEffect(enEffectName_ObjectDrop, m_position, m_rotation, DROP_EFFECT_SCALE);

		//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
		SoundSource* bombSE = NewGO<SoundSource>(0);
		bombSE->Init(enSoundName_ObjBomb);						//初期化
		bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
		bombSE->Play(false);

		//自分が消えるときの処理
		OnDestroy();
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

void ObjectBase::CalcCollision()
{	
	//エネミーとぶつかったかを判定
	for (auto enemy : m_game->GetEnemyList())
	{
		if (m_collisionObject->IsHit(enemy->GetCharaCon()))
		{
			//エネミーはダメージを受けた時の処理を行う
			enemy->HandleDamageEvent(m_damage);

			enemy->PlayReaction();

			//エフェクトの発生
			PlayEffect(enEffectName_ObjectBom, m_position, m_rotation, Vector3::One);

			//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
			SoundSource* bombSE = NewGO<SoundSource>(0);
			bombSE->Init(enSoundName_ObjBomb);						//初期化
			bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
			bombSE->Play(false);

			//自分が消えるときの処理
			OnDestroy();

			//UIの削除
			if (m_targetUI != nullptr)
			{
				DeleteGO(m_targetUI);
			}

			return;
		}

		if (enemy->GetCollision() != nullptr)
		{
			if (m_collisionObject->IsHit(enemy->GetCollision()))
			{
				//エネミーはダメージを受けた時の処理を行う
				enemy->HandleDamageEvent(m_damage);

				//エフェクトの発生
				PlayEffect(enEffectName_ObjectBom, m_position, m_rotation, Vector3::One);

				//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
				SoundSource* bombSE = NewGO<SoundSource>(0);
				bombSE->Init(enSoundName_ObjBomb);						//初期化
				bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
				bombSE->Play(false);

				//自分が消えるときの処理
				OnDestroy();

				//UIの削除
				if (m_targetUI != nullptr)
				{
					DeleteGO(m_targetUI);
				}

				return;
			}
		}	
	}

	//ボスとぶつかったかを判定
	if (m_collisionObject->IsHit(m_boss->GetCharaCon()))
	{
		//ボスはダメージを受けた時の処理を行う
		m_boss->HandleDamageEvent(m_damage);

		//ボスのステートの変更を行う
		m_boss->SetReactionState();

		//エフェクトの発生
		PlayEffect(enEffectName_ObjectBom, m_position, m_rotation, Vector3::One);

		//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
		SoundSource* bombSE = NewGO<SoundSource>(0);
		bombSE->Init(enSoundName_ObjBomb);						//初期化
		bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
		bombSE->Play(false);

		//自分が消えるときの処理
		OnDestroy();

		//UIの削除
		if (m_targetUI != nullptr)
		{
			DeleteGO(m_targetUI);
		}

		return;
	}

	//ステージと衝突したかを計算
	Vector3 length = m_crossPosition - m_position;
	if (length.Length() <= 80.0f)
	{
		//エフェクト発生
		PlayEffect(enEffectName_ObjectDrop, m_position, m_rotation, HIT_STAGE_EFFECT_SCALE);

		//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
		SoundSource* bombSE = NewGO<SoundSource>(0);
		bombSE->Init(enSoundName_ObjBomb);						//初期化
		bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
		bombSE->Play(false);

		//自分が消えるときの処理
		OnDestroy();

		return;
	}
}

void ObjectBase::BombProcess()
{
	//エネミーとぶつかったかを判定
	for (auto enemy : m_game->GetEnemyList())
	{
		if (m_bombCollisionObject->IsHit(enemy->GetCharaCon()))
		{
			//エネミーはダメージを受けた時の処理を行う
			enemy->HandleDamageEvent(m_damage);

			//エフェクトの発生
			PlayEffect(enEffectName_ObjectBom, m_position, m_rotation, Vector3::One);

			//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
			SoundSource* bombSE = NewGO<SoundSource>(0);
			bombSE->Init(enSoundName_ObjBomb);						//初期化
			bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
			bombSE->Play(false);
		}
		else if (enemy->GetCollision() != nullptr)
		{
			if (m_bombCollisionObject->IsHit(enemy->GetCollision()))
			{
				//エネミーはダメージを受けた時の処理を行う
				enemy->HandleDamageEvent(m_damage);

				//エフェクトの発生
				PlayEffect(enEffectName_ObjectBom, m_position, m_rotation, Vector3::One);

				//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
				SoundSource* bombSE = NewGO<SoundSource>(0);
				bombSE->Init(enSoundName_ObjBomb);						//初期化
				bombSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
				bombSE->Play(false);
			}
		}
	}
}

void ObjectBase::CalcAimingDirection()
{
	//デフォルトは引き寄せれない
	m_canAttract = false;

	//デフォルトはモデルを描画する
	m_canSeeModel = true;

	//プレイヤーとの距離が遠すぎたら早期リターン
	Vector3 distance = m_player->GetPosition() - m_position;

	if (distance.Length() >= DRAW_LIMIT)
	{
		//モデルを描画しない
		m_canSeeModel = false;
		return;
	}

	if (distance.Length() > ATTRACT_LIMIT)
	{
		//UIが作成されているなら
		if (m_makeTargetUI == true && m_targetUI != nullptr)
		{
			//削除
			DeleteGO(m_targetUI);

			//UI未作成状態に変更
			m_makeTargetUI = false;
		}

		return;
	}

	//カメラから見えているかチェック
	if (CheckCanSee() == false)
	{
		return;
	}

	//カメラまでの方向を求める
	Vector3 toCameraDir = g_camera3D->GetPosition() - m_position;
	toCameraDir.Normalize();

	//ベクトルを逆向きにする
	toCameraDir *= -1.0f;

	//内積を求める
	float innerProduct = toCameraDir.Dot(g_camera3D->GetForward());

	//内積が１より小さいなら(2つのベクトルが別の方向を向いている)なら
	if (innerProduct < 0.9995f)
	{
		//UIが作成されているなら
		if (m_makeTargetUI == true && m_targetUI != nullptr)
		{
			//削除
			DeleteGO(m_targetUI);

			//UI未作成状態に変更
			m_makeTargetUI = false;
		}

		return;
	}

	//引き寄せ可能にする
	m_canAttract = true;

	//ここまで来てUIがまだないなら
	if (m_makeTargetUI == false)
	{
		//UIの作成
		m_targetUI = NewGO<TargetUI>(0, "targetui");
		m_targetUI->SetObjectPosition(m_position);

		//UI作成済みに変更
		m_makeTargetUI = true;
	}
}

bool ObjectBase::CheckCanSee()
{
	//ソーラーパネルなら
	if (m_imSolarPanel == true)
	{
		return true;
	}

	//コライダーの始点と終点
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//始点はオブジェクトの座標
	start.setOrigin(btVector3(m_position.x, m_position.y + 10.0f, m_position.z));
	//終点はカメラの座標
	end.setOrigin(btVector3(g_camera3D->GetPosition().x, g_camera3D->GetPosition().y, g_camera3D->GetPosition().z));

	//コライダーを始点から終点まで動かして,衝突するかどうかを調べる
	SweepResultWall callback;
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	//壁と衝突したら
	if (callback.isHit == true)
	{
		//モデルは描画しない
		m_canSeeModel = false;
		return false;
	}

	return true;
}

void ObjectBase::PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale)
{
	//エフェクトの再生
	EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(name);
	m_effect->SetPosition(pos);
	m_effect->SetRotation(rot);
	m_effect->SetScale(scale);
	m_effect->Play();
}