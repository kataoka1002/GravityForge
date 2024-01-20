#include "stdafx.h"
#include "HumanEnemyRender.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	bool HumanEnemyRender::Start()
	{
		animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);

		//�C���X�^���V���O�`��p���f���̏�����
		m_modelRender.InitInstancing(
			"Assets/modelData/enemy/humanEnemy.tkm",
			animationClips,
			enAnimClip_Num,
			enModelUpAxisZ,
			false,
			m_maxHumanEnemy
		);
		return true;
	}

	void HumanEnemyRender::Update()
	{
		
	}

	void HumanEnemyRender::Render(RenderContext& rc)
	{
		//m_modelRender.Draw(rc);
	}
}