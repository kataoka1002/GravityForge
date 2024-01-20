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
#include "MakeSound.h"

namespace
{
	const float RUN_SPEED = 500.0f;

	const float WALK_SPEED = 250.0f;

	const float MAX_HP = 100.0f;

	const float HUMAN_ATTACK_DAMAGE = 10.0f;

	const float JUMP_ATTACK_DAMAGE = 50.0f;

	const float MAGIC_ATTACK_DAMAGE = 25.0f;

	const float BOSS_PUNCH_DAMAGE = 5.0f;

	const float BOSS_SWIPE_DAMAGE = 10.0f;
}

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	Player::Player()
	{
		//�A�j���[�V�����̏�����
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

		//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
		m_playerModel.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});

		//�v���C���[���f���̏�����
		m_playerModel.InitInstancing("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_playerModel.SetPosition(m_position);
		m_playerModel.SetRotation(m_rotation);
		m_playerModel.SetScale(m_scale);
		m_playerModel.Update();

		//�L�����N�^�[�R���g���[���[��������
		m_charaCon.Init(
			20.0f,			//���a
			120.0f,			//����
			m_position		//���W
		);

		//�̗͂̐ݒ�
		m_hp = MAX_HP;

		// �����X�e�[�g��ݒ�
		m_playerState = new PlayerIdleState(this);
		m_playerState->Enter();

		//BGM�̐ݒ�ƍĐ�
		m_walkSE = NewGO<SoundSource>(0);
		m_walkSE->Init(enSoundName_PlayerWalk);							//������
		m_walkSE->SetVolume(1.0f * g_soundEngine->GetBgmVolume());		//���ʒ���		

		return true;
	}

	void Player::Update()
	{
		//�Q�[�����ȊO�͑���ł��Ȃ�
		if (m_gameInfo->GetIsInGame() == false || m_gameInfo->GetIsInMovie())
		{
			// �A�j���[�V�������Đ�����B
			PlayAnimation(m_currentAnimationClip);

			// ���f�����X�V����B
			m_playerModel.Update();

			return;
		}

		// �X�e�[�g��ύX���邩
		IPlayerState* playerState = m_playerState->StateChange();

		// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
		if (playerState != nullptr)
		{
			// �X�e�[�g��ύX����B
			delete m_playerState;
			m_playerState = playerState;
			m_playerState->Enter();
		}

		//�{�X�̕ǂɐG��Ă��邩���m�F
		CheckTouchBossWall();

		//�U�����������Ă��邩�`�F�b�N
		DidAttackHit();

		//�̗͂̃`�F�b�N�����Đ����Ă��邩���m�F����
		CheckHP();

		// �e�X�e�[�g�̍X�V���������s�B
		m_playerState->Update();

		//�����̏���
		WalkSEProcess();

		// �A�j���[�V�������Đ�����B
		PlayAnimation(m_currentAnimationClip);

		// ���f�����X�V����B
		m_playerModel.Update();
	}

	void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// �A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void Player::Move()
	{
		//�ړ����x�̏�����
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	
		//���̏�W�����v����Ȃ��Ȃ�
		if (m_isJumping != true)
		{
			//���X�e�B�b�N�̓��͗ʂ��󂯎��
			LStick_x = g_pad[0]->GetLStickXF();
			LStick_y = g_pad[0]->GetLStickYF();

			//�J�����̑O�������ƉE�������擾
			Vector3 cameraForward = g_camera3D->GetForward();
			Vector3 cameraRight = g_camera3D->GetRight();

			//XZ���ʂł̑O�������A�E�����ɕϊ�����
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//�I�u�W�F�N�g�ێ������ǂ����ŕ���������ς���
			float moveSpeed;
			if (m_isHoldingObject)
			{
				moveSpeed = WALK_SPEED;
			}
			else
			{
				moveSpeed = RUN_SPEED;
			}

			//XZ�����̈ړ����x���N���A
			m_moveSpeed += cameraForward * LStick_y * moveSpeed;	//�������ւ̈ړ����x�����Z
			m_moveSpeed += cameraRight * LStick_x * moveSpeed;		//�E�����ւ̈ړ����x�����Z
		}

		//�d�͂̐ݒ�
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		if (m_charaCon.IsOnGround()) 
		{
			//�n�ʂɂ���
			m_moveSpeed.y = 0.0f;

			//���̏�W�����v������Ȃ�
			SetIsJumping(false);
		}
	
		//���W��ݒ�
		m_playerModel.SetPosition(m_position);
	}

	void Player::Turn()
	{	
		//���炩�ɉ��悤�ɂ���
		//�I�u�W�F�N�g�������Ă��鎞
		if (m_isHoldingObject)
		{	
			//�J�����̌��������]�����߂�
			Vector3 rotSpeed = g_camera3D->GetForward();
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, rotSpeed);
		}
		else
		{
			//�ړ����x�����]�����߂�
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 3.5f, m_rotMove, m_moveSpeed);
		}

		//�O�����̐ݒ�
		m_forward = m_rotMove;
		m_forward.Normalize();
	
		//��]��ݒ肷��
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
				//�����񂹂̏�����
				object->InitAttract();

				//�����񂹉\��ԂɕύX
				m_canAttract = true;

				//�J�������ߋ����ɂ���
				m_camera->SetNearCamera(true);

				//�����Ă���I�u�W�F�N�g�̐ݒ�
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

		//�X�e�[�g���U�����ɕύX
		m_canAttack = true;

		//�Ԃ���΂�����
		m_holdingObject->InitBlowAway();

		//�����Ă���I�u�W�F�N�g�͖����ɂ���
		m_holdingObject = nullptr;
	}

	void Player::ResetAttack()
	{
		//�����J�������ߋ����Ȃ�
		if (m_camera->GetIsNearCamera())
		{
			//�J�������������ɖ߂�
			m_camera->SetNearCamera(false);
		}

		//�I�u�W�F�N�g�ێ���Ԃ�����
		m_isHoldingObject = false;

		//�����Ă���I�u�W�F�N�g�͖����ɂ���
		m_holdingObject = nullptr;
	}

	void Player::ChangeWalkingStyle()
	{
		//�X�e�B�b�N�̓��͗ʂɂ���ĕ�������ς���
		if (LStick_x <= -0.5f)
		{
			//�������A�j���[�V����
			SetAnimation(enAnimClip_WalkLeft, 0.3f);
			return;
		}
		else if (LStick_x >= 0.5f)
		{
			//�E�����A�j���[�V����
			SetAnimation(enAnimClip_WalkRight, 0.3f);
			return;
		}

		if (LStick_y <= -0.5f)
		{
			//�������A�j���[�V����
			SetAnimation(enAnimClip_WalkBack, 0.3f);
			return;
		}

		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//�X�^���h�����A�j���[�V����
			SetAnimation(enAnimClip_Standwalk, 0.2f);
		}
		else
		{
			//�X�^���o�C�A�j���[�V����
			SetAnimation(enAnimClip_Standby, 0.2f);
		}
	}

	bool Player::DidAttackHit()
	{
		//�N�[���_�E���̌v�Z
		CalcCoolDown();

		//�N�[���_�E�����Ȃ�Ԃ�
		if (m_isCooldown == true)
		{
			return false;
		}

		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& humanCollisions = g_collisionObjectManager->FindCollisionObjects("human_attack");
		//�z���for���ŉ񂷁B
		for (auto collision : humanCollisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�U���������������̏���
				AttackHitProcess(HUMAN_ATTACK_DAMAGE);

				return true;
			}
		}

		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& bossCollision = g_collisionObjectManager->FindCollisionObjects("boss_jumpAttack");
		//�z���for���ŉ񂷁B
		for (auto collision : bossCollision)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�U���������������̏���
				AttackHitProcess(JUMP_ATTACK_DAMAGE);

				return true;
			}
		}

		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& swipeCollision = g_collisionObjectManager->FindCollisionObjects("swipe_attack");
		//�z���for���ŉ񂷁B
		for (auto collision : swipeCollision)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�U���������������̏���
				AttackHitProcess(BOSS_SWIPE_DAMAGE);

				return true;
			}
		}

		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& punchCollision = g_collisionObjectManager->FindCollisionObjects("punch_attack");
		//�z���for���ŉ񂷁B
		for (auto collision : punchCollision)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�U���������������̏���
				AttackHitProcess(BOSS_PUNCH_DAMAGE);

				return true;
			}
		}

		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& magicCollision = g_collisionObjectManager->FindCollisionObjects("magic_attack");
		//�z���for���ŉ񂷁B
		for (auto collision : magicCollision)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�U���������������̏���
				AttackHitProcess(MAGIC_ATTACK_DAMAGE);

				return true;
			}
		}
		
		return false;
	}

	void Player::AttackHitProcess(float damage)
	{
		//�_���[�W��^����
		CalcDamage(damage);

		//���A�N�V������������
		m_playerState = new PlayerReactionState(this);
		m_playerState->Enter();

		//�N�[���_�E�����ɂ���
		m_isCooldown = true;

		//�I�u�W�F�N�g�������Ă���Ȃ�
		if (m_holdingObject != nullptr)
		{
			//�I�u�W�F�N�g�𗎉����ɂ���
			m_holdingObject->SetFallingState();
		}
	}

	void Player::CalcCoolDown()
	{
		//�N�[���_�E��������Ȃ��Ȃ�
		if (m_isCooldown == false)
		{
			return;
		}

		m_cooldownTime -= g_gameTime->GetFrameDeltaTime();

		if (m_cooldownTime <= 0.0f)
		{
			//�N�[���_�E���I���
			m_isCooldown = false;

			//�^�C�}�[�̃��Z�b�g
			m_cooldownTime = 1.0f;
		}
	}

	void Player::CheckHP()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_hp <= 0.0f)
		{
			m_playerState = new PlayerDeadState(this);
			m_playerState->Enter();
		}
	}

	void Player::CheckTouchBossWall()
	{
		//���łɕǂɐG���Ă�����
		if (m_touchWall == true)
		{
			return;
		}

		//�R���W�������擾����
		const auto& wallCollision = g_collisionObjectManager->FindCollisionObjects("BossFloorWall");
		//�z���for���ŉ�
		for (auto collision : wallCollision)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//�{�X��movie�X�^�[�g
				m_gameInfo->SetInMovie(true);
				m_touchWall = true;
			}
		}
	}

	void Player::CalcDamage(float damage)
	{
		//HP����_���[�W������
		m_hp -= damage;

		if (m_hp <= 0.0f)
		{
			m_hp = 0.0f;
		}
	}

	void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//�L�[�̖��O���uattack_start�v�̎��B
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//�R���W�����I�u�W�F�N�g���쐬����B
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//���W���v���C���[�̏����O,��ɐݒ肷��B
			collisionPosition += m_forward * 70.0f;
			collisionPosition.y += 100.0f;
			//����̃R���W�������쐬����B
			collisionObject->CreateSphere(
				collisionPosition,		//���W�B
				Quaternion::Identity,	//��]�B
				40.0f					//���a�B
			);
			collisionObject->SetName("player_punch");

			//�v���C���[�̍�����ݒ肵�G�t�F�N�g���o��ꏊ���������炷
			Vector3 left = m_forward;
			left.Cross({ 0.0f,1.0f,0.0f });
			Vector3 effectPos = collisionPosition + left * 15.0f;
			
			//�G�t�F�N�g����
			PlayEffect(enEffectName_PlayerPunch, effectPos, m_rotation, Vector3::One);

			//���Đ�����ƏI���Ȃ̂�,�C���X�^���X��ێ������Ȃ��ׂɂ�����NewGO����
			SoundSource* attackSE = NewGO<SoundSource>(0);
			attackSE->Init(enSoundName_PlayerPunch);					//������
			attackSE->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//���ʒ���
			attackSE->Play(false);
		}
		//�L�[�̖��O���uattack_end�v�̎��B
		else if (wcscmp(eventName, L"attack_end") == 0)
		{

		}
	}

	void Player::GameOver()
	{
		//�Q�[���I�[�o�[���Ăяo��
		m_game->SetGameOver();
	}

	void Player::PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale)
	{
		//�G�t�F�N�g�̍Đ�
		EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(name);
		m_effect->SetPosition(pos);
		m_effect->SetRotation(rot);
		m_effect->SetScale(scale);
		m_effect->Play();
	}

	void Player::WalkSEProcess()
	{
		//�I�u�W�F�N�g��ێ����Ă�����,�{�X��̃��[�r�[��,�W�����v���Ȃ瑫���Ȃ�
		if (m_isHoldingObject || m_gameInfo->GetIsInMovie() || m_charaCon.IsOnGround() == false)
		{
			m_walkSE->Stop();
			return;
		}

		//�X�e�B�b�N���͂�����Ȃ�BGM���Đ�����
		if (g_pad[0]->GetLStickXF() == 0.0f && g_pad[0]->GetLStickYF() == 0.0f)
		{
			m_walkSE->Stop();
		}
		else if (m_walkSE->IsPlaying() == false)
		{
			m_walkSE->Play(true);
		}
	}

	void Player::Render(RenderContext& rc)
	{
		m_playerModel.Draw(rc);
	}
}