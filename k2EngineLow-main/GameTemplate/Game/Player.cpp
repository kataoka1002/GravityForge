#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"
#include "GameCamera.h"
#include "Game.h"
#include "ObjectBase.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

namespace
{
	const float MOVE_SPEED = 250.0f;

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
		animationClips[enAnimClip_Walk].Load("Assets/animData/player/player_walk.tka");
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
	}

	Player::~Player()
	{

	}

	bool Player::Start()
	{
		m_camera = FindGO<GameCamera>("gamecamera");

		m_game = FindGO<Game>("game");

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

			//XZ成分の移動速度をクリア
			m_moveSpeed += cameraForward * LStick_y * MOVE_SPEED;	//奥方向への移動速度を加算
			m_moveSpeed += cameraRight * LStick_x * MOVE_SPEED;		//右方向への移動速度を加算
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
		//カメラを遠距離に戻す
		m_camera->SetNearCamera(false);

		//オブジェクト保持状態を解除
		m_isHoldingObject = false;
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

	void Player::Render(RenderContext& rc)
	{
		m_playerModel.Draw(rc);
	}
}