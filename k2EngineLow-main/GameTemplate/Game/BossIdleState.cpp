#include "stdafx.h"
#include "BossIdleState.h"
#include "BossDeadState.h"
#include "BossWalkState.h"
#include "BossMagicState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossIdleState::~BossIdleState()
	{

	}

	void BossIdleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Idle, 0.3f);
	}

	IBossState* BossIdleState::StateChange()
	{
		if (/*m_boss->CheckDistanceToPlayer() >= FOLLOW_RANGE &&*/ m_boss->GetIsMagicCoolDown() != true && m_boss->GetIsHitCoolDown() != true)
		{
			return new BossWalkState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossIdleState::Update()
	{
		//�}�W�b�N�̃N�[���_�E���^�C�����Ȃ�
		if (m_boss->GetIsMagicCoolDown() == true)
		{
			//�N�[���_�E���^�C���̌v�Z
			m_boss->CalcMagicCoolDown();
		}

		//�q�b�g�̃N�[���_�E���^�C�����Ȃ�
		if (m_boss->GetIsHitCoolDown() == true)
		{
			//�N�[���_�E���^�C���̌v�Z
			m_boss->CalcHitCoolDown();
		}
	}
}
