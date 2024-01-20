#pragma once
#include "MakeEffect.h"

//�O���錾
class GameCamera;
class Game;
class ObjectBase;
class Teapot;
class GameInformation;

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	//�O���錾
	class IPlayerState;

	class Player : public IGameObject
	{
	public:
		//�A�j���[�V�����N���b�v
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Walk,
			enAnimClip_Jump,
			enAnimClip_Attract,
			enAnimClip_Standby,
			enAnimClip_Standwalk,
			enAnimClip_Attack,
			enAnimClip_WalkRight,
			enAnimClip_WalkLeft,
			enAnimClip_WalkBack,
			enAnimClip_WalkJump,
			enAnimClip_Reaction,
			enAnimClip_Dead,
			enAnimClip_Punch,
			enAnimClip_Num
		};

	public:
		Player();
		~Player();
		bool Start();
		void Update();
		void Render(RenderContext& rc);
		void Move();
		void Turn();
		void Attract();
		void Attack();
		void ResetAttack();
		void ChangeWalkingStyle();
		void PlayAnimation(EnAnimationClip currentAnimationClip);
		bool DidAttackHit();
		void CalcDamage(float damage);
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// �A�j���[�V�����C�x���g�p�̊֐�
		void GameOver();
		void CheckTouchBossWall();
		void CheckHP();
		void CalcCoolDown();
		void AttackHitProcess(float damage);
		void WalkSEProcess();

		/// <summary>
		/// �G�t�F�N�g�̍Đ�
		/// </summary>
		/// <param name="name">���O</param>
		/// <param name="pos">�|�W�V����</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�傫��</param>
		void PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale);

		/// <summary>
		/// �|�W�V�����̐ݒ�
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3 pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3 scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// �Đ�����A�j���[�V�����N���b�v��ݒ肷��B
		/// </summary>
		/// <param name="enAnimationClip">�A�j���[�V�����N���b�v</param>
		/// <param name="complementTime">��Ԏ���</param>
		void SetAnimation(const EnAnimationClip enAnimationClip, const float complementTime)
		{
			m_currentAnimationClip = enAnimationClip;
			m_complementTime = complementTime;
		}

		/// <summary>
		/// �I�u�W�F�N�g��ێ����Ă��邩�ǂ�����ݒ肷��
		/// </summary>
		/// <param name="isHold"></param>
		void SetIsHoldingObject(const bool isHold)
		{
			m_isHoldingObject = isHold;
		}

		/// <summary>
		/// �����񂹂邱�Ƃ��o���邩��ݒ肷��
		/// </summary>
		/// <param name="canAtt"></param>
		void SetCanAttract(const bool canAtt)
		{
			m_canAttract = canAtt;
		}

		/// <summary>
		/// �U�����邱�Ƃ��o���邩��ݒ肷��
		/// </summary>
		/// <param name="canAtt"></param>
		void SetCanAttack(const bool canAtt)
		{
			m_canAttack = canAtt;
		}

		/// <summary>
		/// Y�����ɗ͂������ăW�����v����
		/// </summary>
		void SetJump()
		{
			m_moveSpeed.y = 400.0f;
		}

		/// <summary>
		/// �W�����v�����ǂ������Z�b�g����
		/// </summary>
		/// <param name="junping"></param>
		void SetIsJumping(bool junping)
		{
			m_isJumping = junping;
		}

		/// <summary>
		/// �|�W�V�����̎擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// ��]�ʂ̎擾
		/// </summary>
		/// <returns></returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// �v���C���[�̑����̎擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPlayerSpeed()const
		{
			return m_moveSpeed;
		}

		/// <summary>
		/// �I�u�W�F�N�g�������Ă��邩�ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetIsHoldingObject()const
		{
			return m_isHoldingObject;
		}

		/// <summary>
		/// �A�j���[�V�����Đ������ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetIsPlayingAnimation() const
		{
			return m_playerModel.IsPlayingAnimation();
		}

		/// <summary>
		/// �����񂹂邱�Ƃ��o���邩�ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetCanAttract()const
		{
			return m_canAttract;
		}

		/// <summary>
		/// �U���ł��邩�ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetCanAttack()const
		{
			return m_canAttack;
		}

		/// <summary>
		/// �n�ʂɒ����Ă��邩�𔻒�
		/// </summary>
		/// <returns></returns>
		const bool& GetOnGround()const
		{
			return m_charaCon.IsOnGround();
		}

		/// <summary>
		/// �v���C���[��HP�̎擾
		/// </summary>
		/// <returns></returns>
		const float& GetHP()const
		{
			return m_hp;
		}

		/// <summary>
		/// HP�̊������擾
		/// </summary>
		/// <returns></returns>
		const float GetHPRatio()const
		{
			return m_hp / 100.0f;
		}


	private:

		AnimationClip		animationClips[enAnimClip_Num];				// �A�j���[�V�����N���b�v
		EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
		ModelRender			m_playerModel;								// �v���C���[���f��
		Vector3				m_position = Vector3::Zero;					// ���W
		Vector3				m_scale = Vector3::One;						// �X�P�[��
		Vector3				m_moveSpeed = Vector3::Zero;				// �ړ����x
		Vector3             m_rotMove = Vector3::Zero;					// ��]���x
		Vector3				m_forward = Vector3::Zero;					// �O����
		CharacterController m_charaCon;									// �L�����N�^�[�R���g���[���[
		Quaternion			m_rotation;									// �N�H�[�^�j�I��
		float				LStick_x = 0.0f;							// L�X�e�B�b�N���͗�(��)
		float				LStick_y = 0.0f;							// L�X�e�B�b�N���͗�(�c)
		float				m_complementTime = 0.0f;					// �A�j���[�V�����̕⊮����
		float				m_hp = 0.0f;								// HP
		float				m_cooldownTime = 1.0f;						// �U�����󂯂�N�[���_�E��
		bool				m_isHoldingObject = false;					// �I�u�W�F�N�g�������Ă��邩�ǂ���
		bool				m_canAttract = false;						// �����񂹂��邩�ǂ���
		bool				m_canAttack = false;						// �U���ł��邩�ǂ���
		bool				m_isJumping = false;						// ���̏�W�����v�����ǂ���
		bool				m_touchWall = false;						// �ǂɐG�������ǂ���
		bool				m_isCooldown = false;						// �N�[���_�E�������ǂ���


		Game* m_game = nullptr;
		GameCamera* m_camera = nullptr;
		ObjectBase* m_holdingObject = nullptr;
		IPlayerState* m_playerState = nullptr;
		GameInformation* m_gameInfo = nullptr;
		SoundSource* m_walkSE = nullptr;								//����
	};
}