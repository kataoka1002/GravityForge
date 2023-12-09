#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"
#include "GameCamera.h"
#include "Game.h"
#include "ObjectBase.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerReactionState.h"
#include "PlayerDeadState.h"
#include "GameInformation.h"

namespace
{
	const float RUN_SPEED = 500.0f;

	const float WALK_SPEED = 250.0f;

	const float MAX_HP = 100.0f;
}

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	Player::Player()
	{
		//アニメーションの初期化
		animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Walk].Load("Assets/animData/player/player_run.tka");
		animationClips[enAnimClip_Walk].SetLoopFlag(true);
		animationClips[enAnimClip_Jump].Load("Assets/animData/player/player_jump.tka");
		animationClips[enAnimClip_Jump].SetLoopFlag(false);
		animationClips[enAnimClip_Attract].Load("Assets/animData/player/player_attract.tka");
		animationClips[enAnimClip_Attract].SetLoopFlag(false);
		animationClips[enAnimClip_Standby].Load("Assets/animData/player/player_standby.tka");
		animationClips[enAnimClip_Standby].SetLoopFlag(true);
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
		animationClips[enAnimClip_WalkJump].Load("Assets/animData/player/player_walk_jump.tka");
		animationClips[enAnimClip_WalkJump].SetLoopFlag(false);
		animationClips[enAnimClip_Dead].Load("Assets/animData/player/player_dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Reaction].Load("Assets/animData/player/player_reaction.tka");
		animationClips[enAnimClip_Reaction].SetLoopFlag(false);	
		animationClips[enAnimClip_Punch].Load("Assets/animData/player/player_punch.tka");
		animationClips[enAnimClip_Punch].SetLoopFlag(false);
	}

	Player::~Player()
	{

	}

	bool Player::Start()
	{
		m_camera = FindGO<GameCamera>("gamecamera");

		m_game = FindGO<Game>("game");

		m_gameInfo = FindGO<GameInformation>("gameinformation");

		//アニメーションイベント用の関数を設定する
		m_playerModel.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});

		//プレイヤーモデルの初期化
		m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_playerModel.SetPosition(m_position);
		m_playerModel.SetRotation(m_rotation);
		m_playerModel.SetScale(m_scale);
		m_playerModel.Update();

		//キャラクターコントローラーを初期化
		m_charaCon.Init(
			20.0f,			//半径
			120.0f,			//高さ
			m_position		//座標
		);

		//体力の設定
		m_hp = MAX_HP;

		// 初期ステートを設定
		m_playerState = new PlayerIdleState(this);
		m_playerState->Enter();

		return true;
	}

	void Player::Update()
	{
		//ゲーム中以外は操作できない
		if (m_gameInfo->GetIsInGame() == false)
		{
			// アニメーションを再生する。
			PlayAnimation(m_currentAnimationClip);

			// モデルを更新する。
			m_playerModel.Update();

			return;
		}

		// ステートを変更するか
		IPlayerState* playerState = m_playerState->StateChange();

		// 変更先のステートが設定されているならば
		if (playerState != nullptr)
		{
			// ステートを変更する。
			delete m_playerState;
			m_playerState = playerState;
			m_playerState->Enter();
		}

		//ボスの壁に触れているかを確認
		CheckTouchBossWall();

		//攻撃が当たっているかチェック
		DidAttackHit();

		//体力のチェックをして生きているかを確認する
		CheckHP();

		// 各ステートの更新処理を実行。
		m_playerState->Update();

		// アニメーションを再生する。
		PlayAnimation(m_currentAnimationClip);

		// モデルを更新する。
		m_playerModel.Update();
	}

	void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// アニメーションを再生
		m_playerModel.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void Player::Move()
	{
		//移動速度の初期化
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	
		//その場ジャンプじゃないなら
		if (m_isJumping != true)
		{
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

			//オブジェクト保持中かどうかで歩く速さを変える
			float moveSpeed;
			if (m_isHoldingObject)
			{
				moveSpeed = WALK_SPEED;
			}
			else
			{
				moveSpeed = RUN_SPEED;
			}

			//XZ成分の移動速度をクリア
			m_moveSpeed += cameraForward * LStick_y * moveSpeed;	//奥方向への移動速度を加算
			m_moveSpeed += cameraRight * LStick_x * moveSpeed;		//右方向への移動速度を加算
		}

		//重力の設定
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//キャラクターコントローラーを使用して座標を更新
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		if (m_charaCon.IsOnGround()) 
		{
			//地面についた
			m_moveSpeed.y = 0.0f;

			//その場ジャンプ中じゃない
			SetIsJumping(false);
		}
	
		//座標を設定
		m_playerModel.SetPosition(m_position);
	}

	void Player::Turn()
	{	
		//滑らかに回るようにする
		//オブジェクトを持っている時
		if (m_isHoldingObject)
		{	
			//カメラの向きから回転を求める
			Vector3 rotSpeed = g_camera3D->GetForward();
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, rotSpeed);
		}
		else
		{
			//移動速度から回転を求める
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 3.5f, m_rotMove, m_moveSpeed);
		}

		//前方向の設定
		m_forward = m_rotMove;
		m_forward.Normalize();
	
		//回転を設定する
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_playerModel.SetRotation(m_rotation);
	}

	void Player::Attract()
	{
		if (m_holdingObject != nullptr)
		{
			return;
		}

		for (auto object : m_game->GetObjectList())
		{
			if (object->GetCanAttract() == true)
			{
				//引き寄せの初期化
				object->InitAttract();

				//引き寄せ可能状態に変更
				m_canAttract = true;

				//カメラを近距離にする
				m_camera->SetNearCamera(true);

				//持っているオブジェクトの設定
				m_holdingObject = object;
				break;
			}
		}
	}

	void Player::Attack()
	{
		if (m_holdingObject == nullptr)
		{
			return;
		}

		//ステートを攻撃中に変更
		m_canAttack = true;

		//ぶっ飛ばす処理
		m_holdingObject->InitBlowAway();

		//持っているオブジェクトは無しにする
		m_holdingObject = nullptr;
	}

	void Player::ResetAttack()
	{
		//もしカメラが近距離なら
		if (m_camera->GetIsNearCamera())
		{
			//カメラを遠距離に戻す
			m_camera->SetNearCamera(false);
		}

		//オブジェクト保持状態を解除
		m_isHoldingObject = false;

		//持っているオブジェクトは無しにする
		m_holdingObject = nullptr;
	}

	void Player::ChangeWalkingStyle()
	{
		//スティックの入力量によって歩き方を変える
		if (LStick_x <= -0.5f)
		{
			//左歩きアニメーション
			SetAnimation(enAnimClip_WalkLeft, 0.3f);
			return;
		}
		else if (LStick_x >= 0.5f)
		{
			//右歩きアニメーション
			SetAnimation(enAnimClip_WalkRight, 0.3f);
			return;
		}

		if (LStick_y <= -0.5f)
		{
			//後ろ歩きアニメーション
			SetAnimation(enAnimClip_WalkBack, 0.3f);
			return;
		}

		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//スタンド歩きアニメーション
			SetAnimation(enAnimClip_Standwalk, 0.2f);
		}
		else
		{
			//スタンバイアニメーション
			SetAnimation(enAnimClip_Standby, 0.2f);
		}
	}

	bool Player::DidAttackHit()
	{
		//クールダウンの計算
		CalcCoolDown();

		//クールダウン中なら返す
		if (m_isCooldown == true)
		{
			return false;
		}

		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& humanCollisions = g_collisionObjectManager->FindCollisionObjects("human_attack");
		//配列をfor文で回す。
		for (auto collision : humanCollisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//攻撃が当たった時の処理
				AttackHitProcess(10.0f);

				return true;
			}
		}

		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& bossCollision = g_collisionObjectManager->FindCollisionObjects("boss_jumpAttack");
		//配列をfor文で回す。
		for (auto collision : bossCollision)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//攻撃が当たった時の処理
				AttackHitProcess(50.0f);

				return true;
			}
		}

		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& swipeCollision = g_collisionObjectManager->FindCollisionObjects("swipe_attack");
		//配列をfor文で回す。
		for (auto collision : swipeCollision)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//攻撃が当たった時の処理
				AttackHitProcess(5.0f);

				return true;
			}
		}

		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& punchCollision = g_collisionObjectManager->FindCollisionObjects("punch_attack");
		//配列をfor文で回す。
		for (auto collision : punchCollision)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//攻撃が当たった時の処理
				AttackHitProcess(5.0f);

				return true;
			}
		}

		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& magicCollision = g_collisionObjectManager->FindCollisionObjects("magic_attack");
		//配列をfor文で回す。
		for (auto collision : magicCollision)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//攻撃が当たった時の処理
				AttackHitProcess(5.0f);

				return true;
			}
		}
		
		return false;
	}

	void Player::AttackHitProcess(float damage)
	{
		//ダメージを与える
		CalcDamage(damage);

		//リアクションをさせる
		m_playerState = new PlayerReactionState(this);
		m_playerState->Enter();

		//クールダウン中にする
		m_isCooldown = true;

		//オブジェクトを持っているなら
		if (m_holdingObject != nullptr)
		{
			//オブジェクトを落下中にする
			m_holdingObject->SetFallingState();
		}
	}

	void Player::CalcCoolDown()
	{
		//クールダウン中じゃないなら
		if (m_isCooldown == false)
		{
			return;
		}

		m_cooldownTime -= g_gameTime->GetFrameDeltaTime();

		if (m_cooldownTime <= 0.0f)
		{
			//クールダウン終わり
			m_isCooldown = false;

			//タイマーのリセット
			m_cooldownTime = 1.0f;
		}
	}

	void Player::CheckHP()
	{
		// 体力が0以下で死亡
		if (m_hp <= 0.0f)
		{
			m_playerState = new PlayerDeadState(this);
			m_playerState->Enter();
		}
	}

	void Player::CheckTouchBossWall()
	{
		//すでに壁に触っていたら
		if (m_touchWall == true)
		{
			return;
		}

		//コリジョンを取得する
		const auto& wallCollision = g_collisionObjectManager->FindCollisionObjects("BossFloorWall");
		//配列をfor文で回す
		for (auto collision : wallCollision)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//ボス戦movieスタート
				m_gameInfo->SetInMovie(true);
				m_touchWall = true;
			}
		}
	}

	void Player::CalcDamage(float damage)
	{
		//HPからダメージを引く
		m_hp -= damage;

		if (m_hp <= 0.0f)
		{
			m_hp = 0.0f;
		}
	}

	void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//キーの名前が「attack_start」の時。
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//コリジョンオブジェクトを作成する。
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//座標をプレイヤーの少し前,上に設定する。
			collisionPosition += m_forward * 70.0f;
			collisionPosition.y += 100.0f;
			//球状のコリジョンを作成する。
			collisionObject->CreateSphere(
				collisionPosition,		//座標。
				Quaternion::Identity,	//回転。
				40.0f					//半径。
			);
			collisionObject->SetName("player_punch");

			//プレイヤーの左側を設定しエフェクトが出る場所を少しずらす
			Vector3 left = m_forward;
			left.Cross({ 0.0f,1.0f,0.0f });
			Vector3 effectPos = collisionPosition + left * 15.0f;
			
			//エフェクト発生
			PlayEffect(enEffectName_PlayerPunch, effectPos, m_rotation, Vector3::One);
		}
		//キーの名前が「attack_end」の時。
		else if (wcscmp(eventName, L"attack_end") == 0)
		{

		}
	}

	void Player::GameOver()
	{
		//ゲームオーバーを呼び出す
		m_game->SetGameOver();
	}

	void Player::PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale)
	{
		//エフェクトの再生
		EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(name);
		m_effect->SetPosition(pos);
		m_effect->SetRotation(rot);
		m_effect->SetScale(scale);
		m_effect->Play();
	}

	void Player::Render(RenderContext& rc)
	{
		m_playerModel.Draw(rc);
	}
}