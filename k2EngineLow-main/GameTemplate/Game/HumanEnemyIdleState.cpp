#include "stdafx.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyAttackState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyIdleState::~HumanEnemyIdleState()
	{

	}

	void HumanEnemyIdleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Idle, 0.5f);
	}

	IHumanEnemyState* HumanEnemyIdleState::StateChange()
	{
		// �̗͂�50�ȉ��Ŏl�񔇂�
		if (m_enemy->GetHP() <= KNEELING_HP)
		{
			return new HumanEnemyDeadState(m_enemy);
		}
		
		//��苗���ȉ��ɂȂ�ƒǂ������Ă���
		if (m_enemy->CheckDistanceToPlayer() <= CHASE_RANGE)
		{
			return new HumanEnemyWalkState(m_enemy);
		}

		//��苗���ȉ��ɂȂ�ƍU�����Ă���
		if (m_enemy->CheckDistanceToPlayer() <= ATTACK_RANGE)
		{
			return new HumanEnemyAttackState(m_enemy);
		}
			
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyIdleState::Update()
	{
		
	}
}