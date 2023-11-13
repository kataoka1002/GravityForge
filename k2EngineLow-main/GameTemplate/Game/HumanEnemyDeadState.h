#pragma once
#include "IHumanEnemyState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	//�O���錾
	class HumanEnemy;

	/// <summary>
	/// �G�l�~�[�̎��S�X�e�[�g�N���X
	/// </summary>
	class HumanEnemyDeadState : public IHumanEnemyState
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="player">�G�l�~�[�̃C���X�^���X�B</param>
		HumanEnemyDeadState(HumanEnemy* enemy) : IHumanEnemyState(enemy) {}
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~HumanEnemyDeadState() override;
		/// <summary>
		/// �X�e�[�g�J�n���̏����B
		/// </summary>
		void Enter() override;
		/// <summary>
		/// �X�e�[�g�̑J�ڏ����B
		/// </summary>
		/// <returns>�J�ڂ���X�e�[�g</returns>
		IHumanEnemyState* StateChange() override;
		/// <summary>
		/// �X�e�[�g�ɂ�����X�V�����B
		/// </summary>
		void Update() override;
	};
}