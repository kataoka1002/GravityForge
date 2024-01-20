#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "GameInformation.h"
#include "Boss.h"
#include "BlackFade.h"
#include "GameStart.h"
#include "MakeSound.h"

namespace
{
	//�����_���王�_�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 150.0f, -300.0f };
	const Vector3 TO_CAMERA_START_POSITION = { 0.0f, 150.0f, 300.0f };

	//�����_���王�_�܂ł̃x�N�g��(�ߋ�����)
	const Vector3 TO_CAMERA_POSITION_NEAR = { 0.0f,100.0f,-270.0f };

	//�����_�̃��[�J���|�W�V����
	const Vector3 TARGET_LOCAL_POSITION = { 0.0f,180.0f,0.0f };

	//�����_�̃��[�J���|�W�V����(�ߋ�����)
	const Vector3 TARGET_LOCAL_POSITION_NEAR = { 60.0f,150.0f,0.0f };

	//�J�����̍ő�ړ����x
	const float MAX_CAMERA_SPEED = 5000.0f;

	//�J�����̓����蔻��̑傫��
	const float CAMERA_COLLISION_SCALE = 5.0f;

	//�J�����̈ړ����x
	const float CAMERA_MOVE_POWER_X = 2.0f;
	const float CAMERA_MOVE_POWER_Y = 1.5f;

	//������
	const float FAR_PLANE = 35000.0f;

	//�{�X�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION_BOSS = { 380.0f, -30.0f, 140.0f };

	//�I�[�v�j���O���̃x�N�g��
	const Vector3 TO_CAMERA_POSITION_OP = { 0.0f, 0.0f, 100.0f };

	//�I�[�v�j���O�̃J�����X�s�[�h
	const float OP_CAMERA_SPEED = 4.0f;

	//�I�[�v�j���O�̎��_�̈ʒu
	const Vector3 OP_CAMERA_POS_1 = { -700.0f,150.0f,100.0f };
	const Vector3 OP_CAMERA_POS_2 = { 240.0f,150.0f,1700.0f };

	//�t�F�[�h���n�܂�܂ł̎���
	const float FADE_START_TIME = 2.0f;

	//�{�X���o��������܂ł̎���
	const float MAKE_BOSS_TIME = 0.7f;

	//�h��̌�����
	const int VIB_DECREASE_AMOUNT = 10;

	//�h��̍ő�l
	const float VIBRATION_MAX = 640.0f;
}

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<nsPlayer::Player>("player");

	//�C���t�H���[�V�����̃C���X�^���X��T���B
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//�{�X�̃C���X�^���X��T��
	m_boss = FindGO<nsBoss::Boss>("boss");

	//�u���b�N�t�F�[�h��T��
	m_blackFade = FindGO<BlackFade>("blackfade");

	//�J�����܂ł̋����ɃZ�b�g
	m_toCameraPos.Set(TO_CAMERA_START_POSITION);

	//�����ʂ̐ݒ�
	g_camera3D->SetFar(FAR_PLANE);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,			//�΂˃J�����̏������s���J�������w�肷��B
		MAX_CAMERA_SPEED,		//�J�����̈ړ����x�̍ő�l�B
		true,					//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		CAMERA_COLLISION_SCALE	//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	//�ŏ��̃^�[�Q�b�g
	m_opTarget = OP_CAMERA_POS_1;

	return true;
}

void GameCamera::OpeningProcess()
{
	//�I�[�v�j���O������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInOpening() == false)
	{
		return;
	}

	//���Ԍv��
	m_opTime += g_gameTime->GetFrameDeltaTime();

	//���_�̐ݒ�
	Vector3 pos = Vector3::Zero;

	//�ꏊ�@�ł̃J�������ړ�
	if (m_opState == enOP_Camera1)
	{
		//�����_���ړ�������
		m_opTarget.x += OP_CAMERA_SPEED;

		//�t�F�[�h�̏���
		CalcOpeningFade(enOP_Camera2, OP_CAMERA_POS_2);

		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	//�ꏊ�A�ł̃J�����ړ�
	else if (m_opState == enOP_Camera2)
	{
		//��ʂ����邭�Ȃ�����
		if (m_blackFade->GetBlackAlpha() <= 0.0f && m_camera1End == false)
		{
			//�t�F�[�h�A�E�g�̏I��
			m_blackFade->SetAlphaDown(false);

			m_camera1End = true;
		}

		//�����_���ړ�������
		m_opTarget.x -= OP_CAMERA_SPEED;
		
		//�t�F�[�h�̏���
		CalcOpeningFade(enOP_Player, CalcTargetPosition());

		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	//�v���C���[�̈ʒu�ł̃J��������
	else if (m_opState == enOP_Player)
	{
		//��ʂ����邭�Ȃ�����
		if (m_blackFade->GetBlackAlpha() <= 0.0f)
		{
			//�t�F�[�h�A�E�g�̏I��
			m_blackFade->SetAlphaDown(false);

			//�I�[�v�j���O�̏I��
			m_gameInfo->SetInOpening(false);

			//�o�g���X�^�[�g��\��(�Q�[���̃X�^�[�g�͂��̒��ōs���Ă���)
			NewGO<GameStart>(0,"gamestart");

			//���Đ�����ƏI���Ȃ̂�,�C���X�^���X��ێ������Ȃ��ׂɂ�����NewGO����
			SoundSource* startSE = NewGO<SoundSource>(0);
			startSE->Init(enSoundName_OpDistortion);					//������
			startSE->SetVolume(1.0f * g_soundEngine->GetBgmVolume());	//���ʒ���
			startSE->Play(false);
		}

		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_START_POSITION;
	}

	//���_�ƒ����_�̐ݒ�
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(m_opTarget);
}

void GameCamera::CalcOpeningFade(const enOPState& nextState, const Vector3& nextPos)
{
	if (m_opTime <= FADE_START_TIME)
	{
		return;
	}
		 
	//�t�F�[�h�C���̊J�n
	m_blackFade->SetAlphaUp(true);

	//��ʂ��^���ÂɂȂ�����
	if (m_blackFade->GetBlackAlpha() >= 1.0f)
	{
		//���̃J�����ꏊ�Ɉړ�
		m_opTarget = nextPos;
		m_springCamera.Refresh();

		//�t�F�[�h�C���̏I��
		m_blackFade->SetAlphaUp(false);

		//�t�F�[�h�A�E�g�̊J�n
		m_blackFade->SetAlphaDown(true);

		//�^�C�}�[�̃��Z�b�g
		m_opTime = 0.0f;

		//���̃X�e�[�g�ֈړ�
		m_opState = nextState;
	}
}

void GameCamera::UpdatePositionAndTarget()
{
	//�Q�[��������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInGame() == false)
	{
		return;
	}

	//���߃J�����̏���
	CalcChangeCamera();

	//�����_�����߂�
	Vector3 target = CalcTargetPosition();

	//�h��̑傫���𑫂�
	target.x += BGX;
	target.y += BGY;

	//���O�̃x�N�g����ۑ�
	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͗�
	float x = 0.0f, y = 0.0f;

	//���߃J�������؂�ւ���Ă��鎞����Ȃ��Ȃ�
	if (m_isChangingCamera != true)
	{
		//�p�b�h�̓��͂��g���ăJ��������
		x = g_pad[0]->GetRStickXF();
		y = g_pad[0]->GetRStickYF();
	}

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, x * CAMERA_MOVE_POWER_X);
	qRot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, y * CAMERA_MOVE_POWER_Y);
	qRot.Apply(m_toCameraPos);

	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	//�J���������������
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	//�J����������������
	else if (toPosDir.y > 0.6f)
	{
		m_toCameraPos = toCameraPosOld;

		//���߃J�����؂�ւ�蒆�Ȃ�
		if (m_isChangingCamera)
		{
			//�J�����̌����𕽍s�ɖ߂�
			qRot.AddRotationDegX(5.0f);
			qRot.Apply(m_toCameraPos);
		}
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::SetNearCamera(bool isNear)
{
	//�J�����̉�]��n��
	Matrix rot = m_springCamera.GetCameraRotation();

	Vector3 toCameraPos;
	
	//�v���C���[���X�^���o�C���Ȃ�
	if (isNear == true)
	{
		//�ߋ����J����ON
		m_isNearCamera = true;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION_NEAR;

		//�J�����̉�]�s��̏C��
		rot.m[0][1] = 1.0f;
		rot.m[1][1] = 0.0f;
		rot.m[2][1] = 0.0f;
		rot.m[3][1] = 0.0f;
	}
	else
	{
		//�ߋ����J����OFF
		m_isNearCamera = false;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION;

		//���߃J�����؂�ւ�蒆�ɂ���
		m_isChangingCamera = true;
	}

	//�J�����̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
	Vector3 localPos = toCameraPos;
	rot.Apply3x3(localPos);

	//�J�����܂ł̋����ɃZ�b�g
	m_toCameraPos.Set(localPos);
}

void GameCamera::CalcChangeCamera()
{
	//���߃J�����؂�ւ�蒆�Ȃ�
	if (m_isChangingCamera)
	{
		//���Ԍv��
		m_cameraChangeTime -= g_gameTime->GetFrameDeltaTime();

		//��莞�Ԍo�߂�
		if (m_cameraChangeTime <= 0.0f)
		{
			//�؂�ւ��I��
			m_isChangingCamera = false;

			//���Ԃ̃��Z�b�g
			m_cameraChangeTime = 0.5f;
		}
	}
}

Vector3 GameCamera::CalcTargetPosition()
{
	//�����_���v�Z����B
	Vector3 target = m_player->GetPosition();

	//�ߋ����J�����̂Ƃ��Ȃ�
	if (m_isNearCamera)
	{
		//�v���C���[�̉�]��n��
		Quaternion rot = m_player->GetRotation();

		//�v���C���[�̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
		Vector3 localPos = TARGET_LOCAL_POSITION_NEAR;
		rot.Multiply(localPos);

		//�v���C���[�̃|�W�V�����Ɍv�Z�������[�J���|�W�V�����𑫂�
		target += localPos;
	}
	else
	{
		//�v���C���[�̑������炿����Ə�𒍎��_�Ƃ���B
		target += TARGET_LOCAL_POSITION;
	}

	//�����O�𒍎��_�ɂ���
	target += g_camera3D->GetForward() * 20.0f;	

	return target;
}

void GameCamera::BossMovieProcess()
{
	//���[�r�[������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInMovie() == false)
	{
		return;
	}

	//�����_�����߂�
	Vector3 target = m_boss->GetPosition();
	target.y = 180.0f;
	target.z -= 20.0f;

	//�h��̑傫���𑫂�
	target.x += BGX;
	target.y += BGY;

	//���_���v�Z����B
	Vector3 pos = target + TO_CAMERA_POSITION_BOSS;
	
	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
	
	//��莞�Ԍo�߂Ń{�X�o��
	m_bossMakeTime += g_gameTime->GetFrameDeltaTime();
	if (m_bossMakeTime >= MAKE_BOSS_TIME)
	{
		//�{�X��X�^�[�g
		m_gameInfo->StartBossBattle();
	}
}

void GameCamera::PlayVibration(int decline, int maxVib, bool& flag, int& vibration)
{
	//�U���̐ݒ�
	int vib = vibration / 2;

	//�h������X�ɏ���������
	vibration -= decline;

	//�h��̑傫����0�ɂȂ�����
	if (vib == 0)
	{
		//�h��ׂ̈ɕK�v�ȕϐ���������
		flag = false;
		BGX = 0; BGY = 0;
		vibration = maxVib;
	}
	else
	{
		//���E�ɗh��邽�߂ɔ����̒l������
		int vibHalf = vib / 2;
		BGX = rand() % vib - vibHalf;
		BGY = rand() % vib - vibHalf;
	}
}

void GameCamera::ManageVibration()
{
	//�h��̃t���O����������
	if (m_vibFlag == true)
	{
		//�h��̍ő�l��ݒ�
		static int Vibration = VIBRATION_MAX;

		//�h��̏���
		PlayVibration(VIB_DECREASE_AMOUNT, VIBRATION_MAX, m_vibFlag, Vibration);
	}
}

void GameCamera::Update()
{
	//�I�[�v�j���O���̏���
	OpeningProcess();

	//�Q�[�����̃J�����̎��_�ƒ����_�̐ݒ�
	UpdatePositionAndTarget();

	//�{�X���[�r�[���̏���
	BossMovieProcess();

	//��ʗh��̏���
	ManageVibration();

	//�J�����̍X�V�B
	m_springCamera.Update();
}

