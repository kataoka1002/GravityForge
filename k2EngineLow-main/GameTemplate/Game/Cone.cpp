#include "stdafx.h"
#include "Cone.h"
#include "Game.h"

namespace
{
	//�R���W�����̑傫��
	const float COLLISION_SCALE = 30.0f;

	//�^����_���[�W
	const float DAMAGE_AMOUNT = 50.0f;

	//�L�����R���̔��a
	const float CHARACON_RADIUS = 20.0f;

	//�L�����R���̍���
	const float CHARACON_HEIGHT = 20.0f;
}

Cone::Cone()
{

}

Cone::~Cone()
{
	DeleteGO(m_collisionObject);
}

void Cone::Update()
{
	//����
	Move();


	m_model.Update();
}

void Cone::InitModel()
{
	m_model.Init("Assets/modelData/object/cone.tkm");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();

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

void Cone::InitCollision()
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
	m_collisionObject->SetName("cone");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Cone::OnDestroy()
{
	//���X�g����폜
	m_game->RemoveObjectFromList(this);

	//�������g�̍폜
	DeleteGO(this);
}

void Cone::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}