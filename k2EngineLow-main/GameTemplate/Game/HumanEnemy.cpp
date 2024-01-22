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
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemy::HumanEnemy()
	{
		//�A�j���[�V�����̏�����
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

		// ��Ɏ���ł��邩������Ȃ��̂ŁA��������null�`�F�b�N������B
		auto render = FindGO<HumanEnemyRender>("humanenemyrender");
		if (render) {
			render->RemoveInstance(m_instanceNo);
		}
	}

	void HumanEnemy::InitModel()
	{
		//m_humanEnemyRender = FindGO<HumanEnemyRender>("humanenemyrender");

		//���f���̏�����
		m_model.Init("Assets/modelData/enemy/humanEnemy2.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_model.SetPosition(m_position);
		m_model.SetRotation(m_rotation);
		m_model.SetScale(m_scale);
		m_model.Update();

		//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
		m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

		// �L�����N�^�[�R���g���[���[��������
		m_charaCon.Init(
			30.0f,			//���a
			150.0f,			//����
			m_position		//���W
		);

		//�ǂɉB��Ă��邩�𔻒肷�邽�߂̃R���C�_
		m_sphereCollider.Create(1.0f);

		// HP�̐ݒ�
		m_hp = MAX_HP;
		m_hpMax = MAX_HP;

		// �����X�e�[�g��ݒ�
		m_humanEnemyState = new HumanEnemyIdleState(this);
		m_humanEnemyState->Enter();

		//UI�̍쐬
		m_enemyUI = NewGO<EnemyUI>(0, "enemyui");
		m_enemyUI->SetEnemy(this);
	}

	void HumanEnemy::Update()
	{
		
		// �X�e�[�g��ύX���邩
		IHumanEnemyState* enemyState = m_humanEnemyState->StateChange();

		// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
		if (enemyState != nullptr)
		{
			// �X�e�[�g��ύX����B
			delete m_humanEnemyState;
			m_humanEnemyState = enemyState;
			m_humanEnemyState->Enter();
		}


		//�v���C���[�̍U���������������𔻒肷��(��������Object�N���X�Ŕ���)
		DidAttackHit();

		//HP���m�F���X�e�[�g��؂�ւ���
		CheckHP();

		// �e�X�e�[�g�̍X�V���������s�B
		m_humanEnemyState->Update();

		// �A�j���[�V�������Đ�����B
		PlayAnimation(m_currentAnimationClip);

		// �v���C���[�Ƃ̋������v�Z����
		CheckDistance();

		// ���f�����X�V����B
		m_model.Update();
		
		m_charaCon.SetPosition(m_position);

		//���f���̍X�V����
		/*m_humanEnemyRender->UpdateInstancingData(
			m_instanceNo,
			m_position,
			m_rotation,
			m_scale
		);*/
	}

	void HumanEnemy::FollowPlayer()
	{
		//�ړ����x�̏�����
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//�v���C���[�̕��������߂�
		Vector3 toPlayerDir = m_player->GetPosition() - m_position;
		toPlayerDir.Normalize();

		//�ŏI�I�Ȉړ����x�̌v�Z
		m_moveSpeed += toPlayerDir * MOVE_SPEED;
		m_moveSpeed.y = 0.0f;

		//�d�͂̐ݒ�
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//���W��ݒ�
		m_model.SetPosition(m_position);
	}

	void HumanEnemy::Turn()
	{
		//�ړ����x�����]�����߂�
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, m_moveSpeed);
		
		//�O�����̐ݒ�
		m_forward = m_rotMove;
		m_forward.Normalize();

		//��]��ݒ肷��
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);
	}

	void HumanEnemy::TurnWhileAttack()
	{
		Vector3 toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();

		//�ړ����x�����]�����߂�
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 3.0f, m_rotMove, toPlayer * 10.0f);

		//�O�����̐ݒ�
		m_forward = m_rotMove;
		m_forward.Normalize();

		//��]��ݒ肷��
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);

		//�v���C���[��Ǐ]����悤�ɂ���
		m_moveSpeed += toPlayer * 0.5f;

		//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//���W��ݒ�
		m_model.SetPosition(m_position);
	}

	void HumanEnemy::OnDestroy()
	{
		//���X�g����폜
		m_game->RemoveEnemyFromList(this);

		//�E�������𑝂₷
		m_gameInfo->AddDefeatEnemyNum();

		//�G�t�F�N�g����
		Vector3 efePos = m_position + m_forward * 50.0f;
		PlayEffect(enEffectName_EnemyBlood, efePos, m_rotation, Vector3::One);

		//���Đ�����ƏI���Ȃ̂�,�C���X�^���X��ێ������Ȃ��ׂɂ�����NewGO����
		SoundSource* deadSE = NewGO<SoundSource>(0);
		deadSE->Init(enSoundName_HumanEnemyDead);				//������
		deadSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//���ʒ���
		deadSE->Play(false);

		//�폜
		DeleteGO(this);
	}

	void HumanEnemy::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// �A�j���[�V�������Đ�
		m_model.SetAnimationSpeed(m_animationSpeed);
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void HumanEnemy::SetCollision()
	{
		//�R���W�����I�u�W�F�N�g���쐬����B
		m_collisionObject = NewGO<CollisionObject>(0);
		Quaternion rot;
		rot.SetRotationDegX(90.0f);

		//�J�v�Z���̃R���W�������쐬����B
		m_collisionObject->CreateCapsule(
			m_position,
			rot,
			30.0f,
			180.0f
		);

		//�R���W�����ɖ��O��t����
		m_collisionObject->SetName("humanEnemy");

		//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	void HumanEnemy::MoveCollision()
	{
		//�R���W���������ɓ|��
		Quaternion rot = m_rotation;
		rot.AddRotationDegX(180.0f);
		
		//�R���W�����̉�],�ړ���ݒ肷��
		m_collisionObject->SetRotation(rot);
		m_collisionObject->SetPosition(m_position);
	}

	void HumanEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//�L�[�̖��O���uattack_start�v�̎��B
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//�R���W�����I�u�W�F�N�g���쐬����B
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//���W���v���C���[�̏����O,��ɐݒ肷��B
			collisionPosition += m_forward * 100.0f;
			collisionPosition.y += 100.0f;
			//����̃R���W�������쐬����B
			collisionObject->CreateSphere(
				collisionPosition,		//���W�B
				Quaternion::Identity,	//��]�B
				50.0f					//���a�B
			);
			collisionObject->SetName("human_attack");

			//�G�t�F�N�g����
			PlayEffect(enEffectName_HumanAttack, collisionPosition, m_rotation, Vector3::One);

			//���Đ�����ƏI���Ȃ̂�,�C���X�^���X��ێ������Ȃ��ׂɂ�����NewGO����
			SoundSource* attackSE = NewGO<SoundSource>(0);
			attackSE->Init(enSoundName_HumanEnemyAttack);				//������
			attackSE->SetVolume(1.0f * g_soundEngine->GetSeVolume());	//���ʒ���
			attackSE->Play(false);
		}
		//�L�[�̖��O���uattack_end�v�̎��B
		else if (wcscmp(eventName, L"attack_end") == 0)
		{
			
		}
	}

	void HumanEnemy::HandleAttackHit()
	{
		// �̗͂����ȉ��Ŏl�񔇂�
		if (m_hp <= KNEELING_HP)
		{
			//�̗�0�ȉ��Ŏ��S
			m_humanEnemyState = new HumanEnemyDeadState(this);
		}
		else
		{
			//�̗͂���Ȃ烊�A�N�V�����̂�
			m_humanEnemyState = new HumanEnemyReactionState(this);
		}

		m_humanEnemyState->Enter();
	}

	void HumanEnemy::CheckHP()
	{
		// �̗͂����ȉ��Ŏl�񔇂�
		if (m_hp > DEAD_LINE && m_hp <= KNEELING_HP && m_isSetDeadState == false)
		{
			m_humanEnemyState = new HumanEnemyDeadState(this);
			m_humanEnemyState->Enter();

			//�Z�b�g�ς݂ɂ���
			m_isSetDeadState = true;
		}

		// �̗͂�0�ȉ��Ŏ��S
		if (m_hp <= DEAD_LINE && m_isSetDieState == false)
		{
			m_humanEnemyState = new HumanEnemyDieState(this);
			m_humanEnemyState->Enter();

			//�Z�b�g�ς݂ɂ���
			m_isSetDieState = true;
		}
	}

	void HumanEnemy::PlayReaction()
	{
		//���A�N�V�����̍Đ�
		m_humanEnemyState = new HumanEnemyReactionState(this);

		m_humanEnemyState->Enter();
	}

	//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		//�Փ˃t���O�B
		bool isHit = false;

		virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			//�ǂƂԂ����ĂȂ�������B
			if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
				//�Փ˂����͕̂ǂł͂Ȃ��B
				return 0.0f;
			}

			//�ǂƂԂ�������t���O��true�ɁB
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
			//�`�悷��
			m_isWithinRange = true;
		}
		else
		{
			//�`�悵�Ȃ�
			m_isWithinRange = false;
			return;
		}

		//�R���C�_�[�̎n�_�ƏI�_
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		//�n�_�̓I�u�W�F�N�g�̍��W
		start.setOrigin(btVector3(m_position.x, m_position.y + 30.0f, m_position.z));
		//�I�_�̓J�����̍��W
		end.setOrigin(btVector3(g_camera3D->GetPosition().x, g_camera3D->GetPosition().y, g_camera3D->GetPosition().z));

		//�R���C�_�[���n�_����I�_�܂œ�������,�Փ˂��邩�ǂ����𒲂ׂ�
		SweepResultWall callback;
		PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

		//�ǂƏՓ˂�����
		if (callback.isHit == true)
		{
			//���f���͕`�悵�Ȃ�
			m_isWithinRange = false;
		}
	}
	
	void HumanEnemy::Render(RenderContext& rc)
	{
		//�J�����O�͈͓̔��Ȃ�
		if (m_isWithinRange != true)
		{
			return;
		}

		m_model.Draw(rc);	
	}
}