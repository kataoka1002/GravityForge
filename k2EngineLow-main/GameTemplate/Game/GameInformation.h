#pragma once
class GameInformation : public IGameObject
{
public:
	~GameInformation();
	bool Start();
	void Update();
	void DeleteBossBGM();

	/// <summary>
	/// �|�������𑝂₷
	/// </summary>
	void AddDefeatEnemyNum()
	{
		m_defeatEnemyNum++;
	}

	/// <summary>
	/// �{�X��ɍs�����Ƃ��o����
	/// </summary>
	void EnableBossBattle()
	{
		m_canGoBossBattle = true;
	}

	/// <summary>
	/// �{�X�풆�ɂ���
	/// </summary>
	void StartBossBattle()
	{
		m_isInBossBattle = true;
	}

	/// <summary>
	/// ���[�r�[�����ǂ������Z�b�g����
	/// </summary>
	/// <param name="now"></param>
	void SetInMovie(bool now)
	{
		m_isInMovie = now;
	}

	/// <summary>
	/// �I�[�v�j���O�����ǂ������Z�b�g����
	/// </summary>
	/// <param name="now"></param>
	void SetInOpening(bool now)
	{
		m_isInOpening = now;
	}

	/// <summary>
	/// �Q�[�������ǂ������Z�b�g����
	/// </summary>
	/// <param name="now"></param>
	void SetInGame(bool now)
	{
		m_isInGame = now;
	}

	/// <summary>
	/// �{�X��ɍs�����Ƃ��ł��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetCanGoBossBattle() const
	{
		return m_canGoBossBattle;
	}

	/// <summary>
	/// �{�X�풆���ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInBossBattle() const
	{
		return m_isInBossBattle;
	}

	/// <summary>
	/// ���[�r�[�����ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInMovie() const
	{
		return m_isInMovie;
	}

	/// <summary>
	/// �I�[�v�j���O�����ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInOpening() const
	{
		return m_isInOpening;
	}

	/// <summary>
	/// �Q�[�������ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInGame() const
	{
		return m_isInGame;
	}

	/// <summary>
	/// �{�X�������Ԃ�Ԃ�
	/// </summary>
	/// <returns></returns>
	const float GetBossKillTime()const
	{
		return m_bossKillTime;
	}

	/// <summary>
	/// �|�����G�̐���Ԃ�
	/// </summary>
	/// <returns></returns>
	const int GetKillEnemyNum()const
	{
		return m_defeatEnemyNum;
	}

private:
	int m_defeatEnemyNum = 0;			//�|�����G�̐�
	float m_bossKillTime = 0.0f;		//�{�X���j����
	bool m_canGoBossBattle = false;		//�{�X��ɍs���邩�ǂ���
	bool m_isInBossBattle = false;		//�{�X�풆���ǂ���
	bool m_isInMovie = false;			//���[�r�[�����ǂ���
	bool m_isInOpening = true;			//�I�[�v�j���O�����ǂ���
	bool m_isInGame = false;			//���C���Q�[�����ǂ���

	SoundSource* m_gameBGM = nullptr;	//�Q�[����BGM
	SoundSource* m_bossBGM = nullptr;	//�{�X��BGM
};