#include "stdafx.h"
#include "HumanEnemy.h"
#include "HumanEnemyConstants.h"
#include "Game.h"
#include "IHumanEnemyState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyReactionState.h"
#include "HumanEnemyDieState.h"
#include "EnemyUI.h"
#include "HumanEnemyRender.h"
#include "MakeSound.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemy::HumanEnemy()
	{
		//アニメーションの初期化
		animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Dead].Load("Assets/animData/enemy/humanEnemy_Dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Crawl].Load("Assets/animData/enemy/humanEnemy_Crawl.tka");
		animationClips[enAnimClip_Crawl].SetLoopFlag(true);
		animationClips[enAnimClip_Tremble].Load("Assets/animData/enemy/humanEnemy_Tremble.tka");
		animationClips[enAnimClip_Tremble].SetLoopFlag(true);
		animationClips[enAnimClip_Die].Load("Assets/animData/enemy/humanEnemy_die.tka");
		animationClips[enAnimClip_Die].SetLoopFlag(false); 
		animationClips[enAnimClip_Walk].Load("Assets/animData/enemy/humanEnemy_walk.tka");
		animationClips[enAnimClip_Walk].SetLoopFlag(true);
		animationClips[enAnimClip_Attack].Load("Assets/animData/enemy/humanEnemy_Attack.tka");
		animationClips[enAnimClip_Attack].SetLoopFlag(false); 
		animationClips[enAnimClip_Reaction].Load("Assets/animData/enemy/humanEnemy_Reaction.tka");
		animationClips[enAnimClip_Reaction].SetLoopFlag(false);
	}

	HumanEnemy::~HumanEnemy()
	{
		DeleteGO(m_collisionObject);
		DeleteGO(m_enemyUI);

		// 先に死んでいるかもしれないので、検索してnullチェックをする。
		auto render = FindGO<HumanEnemyRender>("humanenemyrender");
		if (render) {
			render->RemoveInstance(m_instanceNo);
		}
	}

	void HumanEnemy::InitModel()
	{
		//m_humanEnemyRender = FindGO<HumanEnemyRender>("humanenemyrender");

		//モデルの初期化
		m_model.Init("Assets/modelData/enemy/humanEnemy2.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_model.SetPosition(m_position);
		m_model.SetRotation(m_rotation);
		m_model.SetScale(m_scale);
		m_model.Update();

		//アニメーションイベント用の関数を設定する
		m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

		// キャラクターコントローラーを初期化
		m_charaCon.Init(
			30.0f,			//半径
			150.0f,			//高さ
			m_position		//座標
		);

		//壁に隠れているかを判定するためのコライダ
		m_sphereCollider.Create(1.0f);

		// HPの設定
		m_hp = MAX_HP;
		m_hpMax = MAX_HP;

		// 初期ステートを設定
		m_humanEnemyState = new HumanEnemyIdleState(this);
		m_humanEnemyState->Enter();

		//UIの作成
		m_enemyUI = NewGO<EnemyUI>(0, "enemyui");
		m_enemyUI->SetEnemy(this);
	}

	void HumanEnemy::Update()
	{
		
		// ステートを変更するか
		IHumanEnemyState* enemyState = m_humanEnemyState->StateChange();

		// 変更先のステートが設定されているならば
		if (enemyState != nullptr)
		{
			// ステートを変更する。
			delete m_humanEnemyState;
			m_humanEnemyState = enemyState;
			m_humanEnemyState->Enter();
		}


		//プレイヤーの攻撃が当たったかを判定する(投げ物はObjectクラスで判定)
		DidAttackHit();

		//HPを確認しステートを切り替える
		CheckHP();

		// 各ステートの更新処理を実行。
		m_humanEnemyState->Update();

		// アニメーションを再生する。
		PlayAnimation(m_currentAnimationClip);

		// プレイヤーとの距離を計算する
		CheckDistance();

		// モデルを更新する。
		m_model.Update();
		
		m_charaCon.SetPosition(m_position);

		//モデルの更新処理
		/*m_humanEnemyRender->UpdateInstancingData(
			m_instanceNo,
			m_position,
			m_rotation,
			m_scale
		);*/
	}

	void HumanEnemy::FollowPlayer()
	{
		//移動速度の初期化
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//プレイヤーの方向を求める
		Vector3 toPlayerDir = m_player->GetPosition() - m_position;
		toPlayerDir.Normalize();

		//最終的な移動速度の計算
		m_moveSpeed += toPlayerDir * MOVE_SPEED;
		m_moveSpeed.y = 0.0f;

		//重力の設定
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//キャラクターコントローラーを使用して座標を更新
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//座標を設定
		m_model.SetPosition(m_position);
	}

	void HumanEnemy::Turn()
	{
		//移動速度から回転を求める
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, m_moveSpeed);
		
		//前方向の設定
		m_forward = m_rotMove;
		m_forward.Normalize();

		//回転を設定する
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);
	}

	void HumanEnemy::TurnWhileAttack()
	{
		Vector3 toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();

		//移動速度から回転を求める
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 3.0f, m_rotMove, toPlayer * 10.0f);

		//前方向の設定
		m_forward = m_rotMove;
		m_forward.Normalize();

		//回転を設定する
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);

		//プレイヤーを追従するようにした
		m_moveSpeed += toPlayer * 0.5f;

		//キャラクターコントローラーを使用して座標を更新
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//座標を設定
		m_model.SetPosition(m_position);
	}

	void HumanEnemy::OnDestroy()
	{
		//リストから削除
		m_game->RemoveEnemyFromList(this);

		//殺した数を増やす
		m_gameInfo->AddDefeatEnemyNum();

		//エフェクト発生
		Vector3 efePos = m_position + m_forward * 50.0f;
		PlayEffect(enEffectName_EnemyBlood, efePos, m_rotation, Vector3::One);

		//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
		SoundSource* deadSE = NewGO<SoundSource>(0);
		deadSE->Init(enSoundName_HumanEnemyDead);				//初期化
		deadSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
		deadSE->Play(false);

		//削除
		DeleteGO(this);
	}

	void HumanEnemy::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// アニメーションを再生
		m_model.SetAnimationSpeed(m_animationSpeed);
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void HumanEnemy::SetCollision()
	{
		//コリジョンオブジェクトを作成する。
		m_collisionObject = NewGO<CollisionObject>(0);
		Quaternion rot;
		rot.SetRotationDegX(90.0f);

		//カプセルのコリジョンを作成する。
		m_collisionObject->CreateCapsule(
			m_position,
			rot,
			30.0f,
			180.0f
		);

		//コリジョンに名前を付ける
		m_collisionObject->SetName("humanEnemy");

		//コリジョンオブジェクトが自動で削除されないようにする
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	void HumanEnemy::MoveCollision()
	{
		//コリジョンを横に倒す
		Quaternion rot = m_rotation;
		rot.AddRotationDegX(180.0f);
		
		//コリジョンの回転,移動を設定する
		m_collisionObject->SetRotation(rot);
		m_collisionObject->SetPosition(m_position);
	}

	void HumanEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//キーの名前が「attack_start」の時。
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//コリジョンオブジェクトを作成する。
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//座標をプレイヤーの少し前,上に設定する。
			collisionPosition += m_forward * 100.0f;
			collisionPosition.y += 100.0f;
			//球状のコリジョンを作成する。
			collisionObject->CreateSphere(
				collisionPosition,		//座標。
				Quaternion::Identity,	//回転。
				50.0f					//半径。
			);
			collisionObject->SetName("human_attack");

			//エフェクト発生
			PlayEffect(enEffectName_HumanAttack, collisionPosition, m_rotation, Vector3::One);

			//一回再生すると終わりなので,インスタンスを保持させない為にここでNewGOする
			SoundSource* attackSE = NewGO<SoundSource>(0);
			attackSE->Init(enSoundName_HumanEnemyAttack);				//初期化
			attackSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//音量調整
			attackSE->Play(false);
		}
		//キーの名前が「attack_end」の時。
		else if (wcscmp(eventName, L"attack_end") == 0)
		{
			
		}
	}

	void HumanEnemy::HandleAttackHit()
	{
		// 体力が一定以下で四つん這い
		if (m_hp <= KNEELING_HP)
		{
			//体力0以下で死亡
			m_humanEnemyState = new HumanEnemyDeadState(this);
		}
		else
		{
			//体力あるならリアクションのみ
			m_humanEnemyState = new HumanEnemyReactionState(this);
		}

		m_humanEnemyState->Enter();
	}

	void HumanEnemy::CheckHP()
	{
		// 体力が一定以下で四つん這い
		if (m_hp > DEAD_LINE && m_hp <= KNEELING_HP && m_isSetDeadState == false)
		{
			m_humanEnemyState = new HumanEnemyDeadState(this);
			m_humanEnemyState->Enter();

			//セット済みにする
			m_isSetDeadState = true;
		}

		// 体力が0以下で死亡
		if (m_hp <= DEAD_LINE && m_isSetDieState == false)
		{
			m_humanEnemyState = new HumanEnemyDieState(this);
			m_humanEnemyState->Enter();

			//セット済みにする
			m_isSetDieState = true;
		}
	}

	void HumanEnemy::PlayReaction()
	{
		//リアクションの再生
		m_humanEnemyState = new HumanEnemyReactionState(this);

		m_humanEnemyState->Enter();
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

	void HumanEnemy::CheckDistance()
	{
		Vector3 toPlayerVec = m_player->GetPosition() - m_position;

		float distance = toPlayerVec.Length();

		if (distance <= 5000.0f)
		{
			//描画する
			m_isWithinRange = true;
		}
		else
		{
			//描画しない
			m_isWithinRange = false;
			return;
		}

		//コライダーの始点と終点
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		//始点はオブジェクトの座標
		start.setOrigin(btVector3(m_position.x, m_position.y + 30.0f, m_position.z));
		//終点はカメラの座標
		end.setOrigin(btVector3(g_camera3D->GetPosition().x, g_camera3D->GetPosition().y, g_camera3D->GetPosition().z));

		//コライダーを始点から終点まで動かして,衝突するかどうかを調べる
		SweepResultWall callback;
		PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

		//壁と衝突したら
		if (callback.isHit == true)
		{
			//モデルは描画しない
			m_isWithinRange = false;
		}
	}
	
	void HumanEnemy::Render(RenderContext& rc)
	{
		//カリングの範囲内なら
		if (m_isWithinRange != true)
		{
			return;
		}

		m_model.Draw(rc);	
	}
}