#pragma once
#include "EnemyBase.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	//�O���錾
	class IBossState;
	class BossHitReactionState;
	class BossUI;

	class Boss : public EnemyBase
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Reaction,
			enAnimClip_Jump,
			enAnimClip_Magic,
			enAnimClip_Punch,
			enAnimClip_Swipe,
			enAnimClip_Walk,
			enAnimClip_JumpAttack,
			enAnimClip_Num
		};

	public:
		Boss();
		~Boss();

		void Update();								//�X�V����
		void PlayAnimation(EnAnimationClip currentAnimationClip);	//�A�j���[�V�����̍Đ�
		void SetCollision();						//�R���W�����̐ݒ�
		void MoveCollision();						//�R���W�����̓����̐ݒ�	
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// �A�j���[�V�����C�x���g�p�̊֐�
		void SetReactionState();
		void CheckHP();
		void GameClear();							//�Q�[�����N���A�������ɌĂ΂��
		void CalcMagicCoolDown();					//�}�W�b�N�̃N�[���_�E���^�C���̌v�Z
		void CalcHitCoolDown();						//�q�b�g�̃N�[���_�E���^�C���̌v�Z
		void JumpAttack();
		void Gravity(float pow);
		void SetPlate();
		void MagicAttack();
		void WalkSEProcess();
		bool ToPlayerVecCheck();
		void FollowPlayer()				override;	//�����̏���
		void Turn()						override;	//��]����
		void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
		void InitModel()				override;	//���f���̏�����
		void PlayReaction()				override;	//�q���[�}���G�l�~�[�p�Ȃ̂ł����ł͓��ɈӖ��Ȃ�
		void Render(RenderContext& rc)	override;	//�`�揈��

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
		/// �L�����R���̍Đݒ�
		/// </summary>
		void ResetCharaCon()
		{
			m_charaCon.RemoveRigidBoby();
			m_charaCon.Init(
				30.0f,			//���a
				10.0f,			//����
				m_position		//���W
			);
		}

		/// <summary>
		/// �A�j���[�V�����Đ������ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetIsPlayingAnimation() const
		{
			return m_model.IsPlayingAnimation();
		}

		/// <summary>
		/// �̗͂̎擾
		/// </summary>
		/// <returns></returns>
		const float GetHP()const
		{
			return m_hp;
		}

		/// <summary>
		/// �v���C���[�܂ł̋����𑪂�
		/// </summary>
		/// <returns></returns>
		float CheckDistanceToPlayer()
		{
			//�v���C���[�܂ł̃x�N�g�������ߋ��������߂�
			Vector3 toPlayer = m_player->GetPosition() - m_position;
			float toPlayerLen = toPlayer.Length();
			return toPlayerLen;
		}

		/// <summary>
		/// HP�̊������擾
		/// </summary>
		/// <returns></returns>
		const float GetHPRatio()const
		{
			return m_hp / 100.0f;
		}

		/// <summary>
		/// �}�W�b�N�̃N�[���_�E���^�C�������ǂ�����Ԃ�
		/// </summary>
		/// <returns></returns>
		bool GetIsMagicCoolDown()
		{
			return m_isMagicCoolDowm;
		}

		/// <summary>
		/// �}�W�b�N�̃N�[���_�E���^�C�����ɂ���
		/// </summary>
		void SetMagicCoolDown()
		{
			m_isMagicCoolDowm = true;
		}

		/// <summary>
		/// �q�b�g�̃N�[���_�E���^�C�������ǂ�����Ԃ�
		/// </summary>
		/// <returns></returns>
		bool GetIsHitCoolDown()
		{
			return m_isHitCoolDowm;
		}

		/// <summary>
		/// �q�b�g�̃N�[���_�E���^�C�����ɂ���
		/// </summary>
		void SetHitCoolDown()
		{
			m_isHitCoolDowm = true;
		}

		/// <summary>
		/// �W�����v�U���̃^�[�Q�b�g��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetJumpTarget()
		{
			m_jumpTargetPos = m_player->GetPosition();
		}

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// �A�j���[�V�����N���b�v
		EnAnimationClip		m_currentAnimationClip;					// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
		IBossState*			m_bossState = nullptr;					// �X�e�[�g	
		float				m_complementTime = 0.0f;				// �A�j���[�V�����̕⊮����
		float				m_magicCoolDowmTime = 0.0f;				// �}�W�b�N�̃N�[���_�E���^�C��
		bool				m_isMagicCoolDowm = false;				// �}�W�b�N�̃N�[���_�E���^�C�����ǂ���
		float				m_hitCoolDowmTime = 0.0f;				// �q�b�g�̃N�[���_�E���^�C��
		bool				m_isHitCoolDowm = false;				// �q�b�g�̃N�[���_�E���^�C�����ǂ���
		bool				m_isJumping = false;					// �W�����v�����ǂ���
		Vector3				m_jumpTargetPos = Vector3::Zero;		// �W�����v�U���̖ړI�n
		float				m_plateDrawTime = 2.0f;
		bool				m_magicAttackNow = false;				// �}�W�b�N�����ǂ���
		bool				m_makeMagicEffe = false;				// �}�W�b�N�̃G�t�F�N�g����������ǂ���
		bool				m_isPlayNameSE = false;					// ���O�̌��ʉ����Đ��������ǂ���
		bool				m_isWalking = false;					// �����Ă��邩�ǂ���

		BossUI*				m_ui = nullptr;
		SoundSource*		m_walkSE = nullptr;						// ����
	};
}
