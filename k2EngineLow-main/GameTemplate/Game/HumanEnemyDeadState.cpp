#include "stdafx.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyTrembleState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyDeadState::~HumanEnemyDeadState()
	{

	}

	void HumanEnemyDeadState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Dead, 0.5f);
	}

	IHumanEnemyState* HumanEnemyDeadState::StateChange()
	{
		// �A�j���[�V�����̍Đ����I�������l�񔇂�
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//�A�j���[�V�������x�����Ƃɂ��ǂ�
			m_enemy->SetAnimationSpeed(1.0f);

			//�L�����R���̍Đݒ���s��
			m_enemy->ResetCharaCon();

			//�R���W�����̐ݒ�
			//m_enemy->SetCollision();

			//�l�񔇂���true�ɂ���
			m_enemy->SetCrawlNow();

			return new HumanEnemyTrembleState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyDeadState::Update()
	{
		
	}
}
