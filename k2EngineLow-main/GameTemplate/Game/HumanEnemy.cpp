#include "stdafx.h"
#include "HumanEnemy.h"
#include "HumanEnemyConstants.h"
#include "Game.h"
#include "IHumanEnemyState.h"
#include "HumanEnemyIdleState.h"
#include "EnemyUI.h"

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
	}

	void HumanEnemy::InitModel()
	{
		//モデルの初期化
		m_model.Init("Assets/modelData/enemy/humanEnemy.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
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

		// 各ステートの更新処理を実行。
		m_humanEnemyState->Update();

		// アニメーションを再生する。
		PlayAnimation(m_currentAnimationClip);

		// モデルを更新する。
		m_model.Update();
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

	void HumanEnemy::OnDestroy()
	{
		//リストから削除
		m_game->RemoveEnemyFromList(this);

		//削除
		DeleteGO(this);
	}

	void HumanEnemy::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// アニメーションを再生
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
		}
		//キーの名前が「attack_end」の時。
		else if (wcscmp(eventName, L"attack_end") == 0)
		{
			
		}
	}

	void HumanEnemy::Render(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}