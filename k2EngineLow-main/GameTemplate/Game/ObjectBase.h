#pragma once
#include "TargetUI.h"
#include "MakeEffect.h"

// �O���錾�B
namespace nsPlayer { class Player; }
namespace nsBoss { class Boss; }
class Game;

class ObjectBase : public IGameObject, Noncopyable
{
public:

	//�R���X�g���N�^
	ObjectBase() {}
	//�f�X�g���N�^�̓o�[�`�����ɂ��Ă���
	virtual ~ObjectBase() {}
	bool Start();
	/// <summary>
	/// �����񂹏����̏�����
	/// </summary>
	void InitAttract();

	/// <summary>
	/// �Ԃ���я����̏�����
	/// </summary>
	void InitBlowAway();

	/// <summary>
	/// �������̏���
	/// </summary>
	void FallingObject();

	/// <summary>
	/// �n�ʂɗ����������ǂ����̔���
	/// </summary>
	void CheckToLand();

	/// <summary>
	/// �v���C���[���猩���Ă��邩�`�F�b�N
	/// </summary>
	/// <returns>true : ������ , false : �����Ȃ�</returns>
	bool CheckCanSee();

	/// <summary>
	/// ��юn�߂Ă���̎��Ԃ��v�Z
	/// </summary>
	void CheckFlightTime();

	virtual void Render(RenderContext& rc)	= 0;	//�`�揈��
	virtual void InitModel()				= 0;	//���f���̏�����
	virtual void OnDestroy()				= 0;	//������Ƃ��ɌĂ΂�鏈��
	virtual void InitCollision()			= 0;	//�R���W�����̏�����

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
	/// �ړI�n�̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetTargetPos(const Vector3 pos)
	{
		 m_targetPosition = pos;
	}

	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="vel"></param>
	void SetVelocity(Vector3 vel)
	{
		m_velocity = vel;
	}

	/// <summary>
	/// �X�e�[�g�𗎉����ɂ���
	/// </summary>
	void SetFallingState()
	{
		m_objectState = enObjectState_Falling;
	}

	/// <summary>
	/// �����񂹂�邩�ǂ����̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetCanAttract() const
	{
		return m_canAttract;
	}

	/// <summary>
	/// �C���X�^���X�ԍ���ݒ�B
	/// </summary>
	/// <param name="instanceNo"></param>
	void SetInstanceNo(int instanceNo)
	{
		m_instanceNo = instanceNo;
	}
	
protected:

	/// <summary>
	/// �v���C���[�܂ň����񂹂��鏈��
	/// </summary>
	void AttractedToPlayer();

	/// <summary>
	/// �����񂹒��̑��x�v�Z
	/// </summary>
	/// <param name="speed"></param>
	/// <param name="curvatureRadius"></param>
	/// <param name="damping"></param>
	void CalcVelocity(const float speed, const float curvatureRadius, const float damping);

	/// <summary>
	/// �����̏���
	/// </summary>
	void Move();

	/// <summary>
	/// �A�C�h�����̏���
	/// </summary>
	void IdleMove();

	/// <summary>
	/// �ǔ����̏���
	/// </summary>
	void FollowPlayer();

	/// <summary>
	/// �t���t������
	/// </summary>
	void UpDown();

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="speed"></param>
	void Turn(Vector3 speed);

	/// <summary>
	/// �A�C�h�����̖ړI�n�̌v�Z
	/// </summary>
	void CalcTargetPosition();

	/// <summary>
	/// ������ԏ���
	/// </summary>
	void BlowAway();

	/// <summary>
	/// ���ɓ������Ă��邩�̏���
	/// </summary>
	void CalcCollision();

	/// <summary>
	/// ���e�B�N���ɍ��킳���Ă��邩�̏���
	/// </summary>
	void CalcAimingDirection();

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�傫��</param>
	void PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// �����̏���
	/// </summary>
	void BombProcess();

protected:
	enum EnObjectState
	{
		enObjectState_Quiescence,	//�������
		enObjectState_Idle,			//�ҋ@��
		enObjectState_Attract,		//�����񂹒�
		enObjectState_Blow,			//������ђ�
		enObjectState_Falling,		//������
		enObjectState_Num
	};

	ModelRender				m_model;									//���f��
	CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[
	SphereCollider			m_sphereCollider;							//�Օ����m�F�p�̃R���C�_�[
	Vector3					m_position = Vector3::Zero;					//�|�W�V����
	Vector3					m_targetPosition = Vector3::Zero;			//�ړI�n
	Vector3					m_velocity = Vector3::Zero;					//���x
	Vector3					m_rotMove = Vector3::Zero;					//��]���x
	Vector3					m_flightSpeed = Vector3::Zero;				//���ł�����
	Vector3					m_followSpeed = Vector3::Zero;				//�ǔ��̑���
	Vector3					m_scale = Vector3::Zero;					//�傫��
	Vector3					m_collisionPosition = Vector3::Zero;		//�R���W�����I�u�W�F�N�g�̍��W
	Vector3					m_flightDir = Vector3::Zero;				//���ł�������
	Vector3					m_crossPosition = Vector3::Zero;			//���C�Ƃ̌�_�̍��W
	Vector3					m_localPosition = Vector3::Zero;			//�����Ă��鎞�̃��[�J�����W
	Quaternion				m_rotation;									//�N�H�[�^�j�I��
	EnObjectState			m_objectState = enObjectState_Quiescence;	//�I�u�W�F�N�g�̃X�e�[�g(���)��\���ϐ�
	float					m_degree = 0.0f;							//�t���t���p�̊p�x
	float					m_damage = 0.0f;							//�^����_���[�W
	float					m_collisionAddPos = 0.0f;					//�R���W�����I�u�W�F�N�g��Y�l���Z��
	float					m_diagonalRot = 0.0f;						//�΂߂̊p�x
	float					m_fallingTime = 0.0f;						//�������n�߂Ă���̎���
	float					m_flightTime = 0.0f;						//��юn�߂Ă���̎���
	bool					m_canAttract = false;						//�����񂹂�邩�ǂ���
	bool					m_makeTargetUI = false;						//targetUI����������ǂ���
	bool					m_imSolarPanel = false;						//�\�[���[�p�l�����ǂ���
	bool					m_canSeeModel = true;						//���f����`�悷�邩�ǂ���
	int						m_smokeEfeInterval = 0;						//���G�t�F�N�g����������Ԋu
	int						m_sandCount = 0;
	int						m_instanceNo = 0;							// �C���X�^���X�ԍ��B

	nsPlayer::Player*		m_player = nullptr;
	nsBoss::Boss*			m_boss = nullptr;
	Game*					m_game = nullptr;
	CollisionObject*		m_collisionObject = nullptr;
	CollisionObject*		m_bombCollisionObject = nullptr;
	TargetUI*				m_targetUI = nullptr;
};

