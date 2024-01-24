#include "stdafx.h"
#include "BossMagicState.h"
#include "BossIdleState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossMagicState::~BossMagicState()
	{

	}

	void BossMagicState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Magic, 0.3f);
		m_boss->SetAnimationSpeed(0.7f);
	}
	
	IBossState* BossMagicState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//�}�W�b�N�̃N�[���_�E���^�C�����ɂ���
			m_boss->SetMagicCoolDown();

			return new BossIdleState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossMagicState::Update()
	{
		//�}�W�b�N�U���̏���
		m_boss->MagicAttack();
	}
}
