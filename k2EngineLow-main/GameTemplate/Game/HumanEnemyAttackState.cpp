#include "stdafx.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyReactionState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyAttackState::~HumanEnemyAttackState()
	{

	}

	void HumanEnemyAttackState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Attack, 0.2f);
		//�A�j���[�V�������x���グ��
		m_enemy->SetAnimationSpeed(2.0f);
	}

	IHumanEnemyState* HumanEnemyAttackState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//�A�j���[�V�������x�����Ƃɂ��ǂ�
			m_enemy->SetAnimationSpeed(1.0f);

			return new HumanEnemyIdleState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		//��]����
		m_enemy->TurnWhileAttack();
	}
}
