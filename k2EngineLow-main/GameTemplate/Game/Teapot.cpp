#include "stdafx.h"
#include "Teapot.h"
#include "Game.h"
#include "TeapotRender.h"
namespace
{
	//�R���W�����̑傫��
	const float COLLISION_RADIUS = 80.0f;

	//�R���W�����̍���
	const float COLLISION_HEIGHT = 200.0f;

	//�����R���W�����̑傫��
	const float BOMB_COLLISION_SCALE = 300.0f;

	//�^����_���[�W
	const float DAMAGE_AMOUNT = 100.0f;

	//�L�����R���̔��a
	const float CHARACON_RADIUS = 10.0f;

	//�L�����R���̍���
	const float CHARACON_HEIGHT = 10.0f;
}

Teapot::Teapot()
{

}

Teapot::~Teapot()
{
	DeleteGO(m_collisionObject);
	DeleteGO(m_bombCollisionObject);
	DeleteGO(m_carCollision);
	// ��Ɏ���ł��邩������Ȃ��̂ŁA��������null�`�F�b�N������B
	auto render = FindGO<TeapotRender>("teapotrender");
	if (render) {
		render->RemoveInstance(m_instanceNo);
	}
}

void Teapot::Update()
{
	//����
	Move();

	//�����グ��ꂽ��R���W�����𖳌�������
	if (m_objectState != enObjectState_Quiescence)
	{
		m_carCollision->SetIsEnable(false);
	}

	//m_model.Update();

	//���f���̍X�V����
	m_teapotRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void Teapot::InitModel()
{
	m_teapotRender = FindGO<TeapotRender>("teapotrender");

	/*m_model.Init("Assets/modelData/object/teapot.tkm");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();*/

	//�L�����N�^�[�R���g���[���[��������
	m_charaCon.Init(
		CHARACON_RADIUS,	//���a
		CHARACON_HEIGHT,	//����
		m_position			//���W
	);

	//�Ԃ̃R���W�����̏�����
	InitCarCollision();

	//�^����_���[�W�̐ݒ�
	m_damage = DAMAGE_AMOUNT;

	m_localPosition = { 150.0f,200.0f,0.0f };
}

void Teapot::InitCarCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_carCollision = NewGO<CollisionObject>(0);

	//�R���W���������ɓ|��
	Quaternion rot = m_rotation;
	rot.AddRotationDegX(180.0f);

	//�J�v�Z����̃R���W�������쐬����B
	m_carCollision->CreateCapsule(
		m_position,				//���W
		rot,					//��]
		COLLISION_RADIUS,		//���a
		COLLISION_HEIGHT		//����
	);

	//�R���W�����ɖ��O��t����
	m_carCollision->SetName("carcollision");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_carCollision->SetIsEnableAutoDelete(false);
}

void Teapot::InitCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);

	//�R���W���������ɓ|��
	Quaternion rot = m_rotation;
	rot.AddRotationDegX(180.0f);

	//�J�v�Z����̃R���W�������쐬����B
	m_collisionObject->CreateCapsule(
		m_position,				//���W
		rot,					//��]
		COLLISION_RADIUS,		//���a
		COLLISION_HEIGHT		//����
	);

	//�R���W�����ɖ��O��t����
	m_collisionObject->SetName("teapot");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Teapot::OnDestroy()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_bombCollisionObject = NewGO<CollisionObject>(0);

	//����̃R���W�������쐬����B
	m_bombCollisionObject->CreateSphere(
		m_position,				//���W
		Quaternion::Identity,	//��]
		BOMB_COLLISION_SCALE	//���a
	);

	//�R���W�����ɖ��O��t����
	m_bombCollisionObject->SetName("bomb");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_bombCollisionObject->SetIsEnableAutoDelete(false);

	//�����̓����蔻��̏���
	BombProcess();

	//�G�t�F�N�g�̔���
	PlayEffect(enEffectName_BombLightning, m_position, Quaternion::Identity, Vector3::One);
	PlayEffect(enEffectName_BombLightning, m_position, Quaternion::Identity, Vector3::One);

	//���X�g����폜
	m_game->RemoveObjectFromList(this);

	//�������g�̍폜
	DeleteGO(this);
}

void Teapot::Render(RenderContext& rc)
{
	//���f���������Ȃ��ʒu�ɂ���Ȃ�`�悵�Ȃ�
	if (m_canSeeModel != true)
	{
		return;
	}

	//m_model.Draw(rc);
}