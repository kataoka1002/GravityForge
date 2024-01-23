#include "stdafx.h"
#include "BossWalkState.h"
#include "BossSwipeState.h"
#include "BossPunchState.h"
#include "BossMagicState.h"
#include "BossJumpAttackState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossWalkState::~BossWalkState()
	{

	}

	void BossWalkState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Walk, 0.3f);
	}

	IBossState* BossWalkState::StateChange()
	{
		if (m_boss->CheckDistanceToPlayer() < FOLLOW_RANGE && m_boss->ToPlayerVecCheck())
		{
			//�����_���Ȓl�����
			int num = std::rand() % 2 + 1;

			if (num == 1)
			{
				//�؂�􂫍U��
				return new BossSwipeState(m_boss);
			}
			if (num == 2)
			{
				//�p���`
				return new BossPunchState(m_boss);
			}
		}

		if (std::rand() % 300 + 1 == 1)
		{
			return new BossJumpAttackState(m_boss);
		}

		if (std::rand() % 500 + 1 == 1)
		{
			return new BossMagicState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossWalkState::Update()
	{
		//�v���C���[��ǂ�������
		m_boss->FollowPlayer();

		//��]����
		m_boss->Turn();
	}
}
