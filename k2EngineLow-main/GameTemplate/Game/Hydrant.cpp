#include "stdafx.h"
#include "Hydrant.h"
#include "Game.h"
#include "HydrantRender.h"

namespace
{
	//�R���W�����̑傫��
	const float COLLISION_SCALE = 80.0f;

	//�^����_���[�W
	const float DAMAGE_AMOUNT = 30.0f;

	//�L�����R���̔��a
	const float CHARACON_RADIUS = 20.0f;

	//�L�����R���̍���
	const float CHARACON_HEIGHT = 10.0f;
}

Hydrant::Hydrant()
{

}

Hydrant::~Hydrant()
{
	DeleteGO(m_collisionObject);
	// ��Ɏ���ł��邩������Ȃ��̂ŁA��������null�`�F�b�N������B
	auto render = FindGO<HydrantRender>("hydrantrender");
	if (render) {
		render->RemoveInstance(m_instanceNo);
	}
}

void Hydrant::Update()
{
	//����
	Move();

	//m_model.Update();

	//���f���̍X�V����
	m_hydrantRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void Hydrant::InitModel()
{
	m_hydrantRender = FindGO<HydrantRender>("hydrantrender");

	//m_model.Init("Assets/modelData/object/hydrant.tkm");
	//m_model.SetPosition(m_position);
	//m_model.SetRotation(m_rotation);
	//m_model.SetScale(m_scale);
	//m_model.Update();

	//�L�����N�^�[�R���g���[���[��������
	m_charaCon.Init(
		CHARACON_RADIUS,	//���a
		CHARACON_HEIGHT,	//����
		m_position			//���W
	);

	//�^����_���[�W�̐ݒ�
	m_damage = DAMAGE_AMOUNT;

	//�R���W�������o�����W�̉��Z�ʂ����߂�
	if (CHARACON_RADIUS >= CHARACON_HEIGHT)
	{
		m_collisionAddPos = CHARACON_RADIUS;
	}
	else
	{
		m_collisionAddPos = CHARACON_HEIGHT;
	}
}

void Hydrant::InitCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);

	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(
		m_position,				//���W
		Quaternion::Identity,	//��]
		COLLISION_SCALE			//���a
	);

	//�R���W�����ɖ��O��t����
	m_collisionObject->SetName("hydrant");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Hydrant::OnDestroy()
{
	//���X�g����폜
	m_game->RemoveObjectFromList(this);

	//�������g�̍폜
	DeleteGO(this);
}

void Hydrant::Render(RenderContext& rc)
{
	//���f���������Ȃ��ʒu�ɂ���Ȃ�`�悵�Ȃ�
	if (m_canSeeModel != true)
	{
		return;
	}

	//m_model.Draw(rc);
}