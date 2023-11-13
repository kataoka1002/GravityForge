#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerAttractState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "PlayerDeadState.h"
#include "PlayerReactionState.h"

namespace
{

}

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerIdleState::~PlayerIdleState()
	{

	}

	void PlayerIdleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Idle, 0.5f);
	}

	IPlayerState* PlayerIdleState::StateChange()
	{
		if (m_player->GetCanAttract()) 
		{
			// �����񂹕s�̏�Ԃɖ߂�
			m_player->SetCanAttract(false);

			// �����񂹃X�e�[�g�ɑJ�ڂ���B
			return new PlayerAttractState(m_player);
		}

		//�X�e�B�b�N�̓��͗ʂɂ���ăX�e�[�g��ύX
		if(g_pad[0]->GetLStickXF() != 0.000f || g_pad[0]->GetLStickYF() != 0.000f)
		{
			// �����X�e�[�g�ɑJ�ڂ���
			return new PlayerWalkState(m_player);
		}

		if (g_pad[0]->IsTrigger(enButtonX))
		{		
			//�W�����v����
			m_player->SetJump();
			
			// �W�����v�X�e�[�g�ɑJ�ڂ���
			return new PlayerJumpState(m_player);
		}

		//�U�����󂯂���
		if (m_player->DidAttackHit())
		{
			if (m_player->GetHP() <= 0.0f)
			{
				//�̗�0�ȉ��Ŏ��S
				return new PlayerDeadState(m_player);
			}
			else
			{
				//�̗͂���Ȃ烊�A�N�V�����̂�
				return new PlayerReactionState(m_player);
			}
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerIdleState::Update()
	{		
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�����񂹂鏈��
			m_player->Attract();
		}
	}
}