#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"
#include "GameInformation.h"
#include "GameCamera.h"

bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");
	m_gameInfo = FindGO<GameInformation>("gameinformation");
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	//���f���̏�����
	InitModel();

	return true;
}

void EnemyBase::HandleDamageEvent(float damage)
{
	//�_���[�W��^����
	m_hp -= damage;
	m_hp = max(0.0f, m_hp);
}

bool EnemyBase::DidAttackHit()
{
	//�N�[���_�E���̌v�Z
	CalcCoolDown();

	//�N�[���_�E�����Ȃ�Ԃ�
	if (m_isCooldown == true)
	{
		return false;
	}

	//�U���p�̃R���W�����̔z����擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_punch");
	//�z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//�_���[�W��^����
			HandleDamageEvent(10.0f);

			//���A�N�V�����̍Đ�
			PlayReaction();

			//�N�[���_�E�����ɂ���
			m_isCooldown = true;

			return true;
		}
	}

	return false;
}

void EnemyBase::CalcCoolDown()
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

void EnemyBase::PlayEffect(EffectName name, Vector3 pos, Quaternion rot, Vector3 scale)
{
	//�G�t�F�N�g�̍Đ�
	EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(name);
	m_effect->SetPosition(pos);
	m_effect->SetRotation(rot);
	m_effect->SetScale(scale);
	m_effect->Play();
}