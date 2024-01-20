#pragma once
class ResultCamera;
class BlackFade;
class GameInformation;

class Result : public IGameObject, Noncopyable
{
public:
	//�A�j���[�V�����N���b�v
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Dwarf,
		enAnimClip_Num
	};

public:
	Result() {}
	virtual ~Result() {}

	bool Start();
	void GoTitle();
	void SetAnimation();
	virtual void InitCamera() = 0;	//���f���̏�����
	virtual void OnDestroy() = 0;	//������Ƃ��ɌĂ΂�鏈��
	virtual void InitUI() = 0;

	/// <summary>
	/// �v���C���[���f���̃|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPlayerPosition() const
	{
		return m_playerPosition;
	}

protected:
	AnimationClip		animationClips[enAnimClip_Num];				// �A�j���[�V�����N���b�v
	EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
	ModelRender			m_playerModel;								// �v���C���[���f��
	ModelRender			m_bgModel;									// �w�i�̃��f��
	ResultCamera*		m_resultCamera = nullptr;					// �J����
	BlackFade*			m_blackFade = nullptr;						// �t�F�[�h
	SoundSource*		m_resultBGM = nullptr;						// ���U���g��BGM
	GameInformation*	m_gameInfo = nullptr;

	Vector3				m_playerPosition = Vector3::Zero;			// �v���C���[�̃|�W�V����
	float				m_time = 0.0;								// �A�j���[�V�������Đ�����܂ł̎���
	bool				m_didPlayAnim = false;						// �A�j���[�V�����Đ����������ǂ���
	bool				m_pressAButton = false;						// A�{�^�������������ǂ���
};

