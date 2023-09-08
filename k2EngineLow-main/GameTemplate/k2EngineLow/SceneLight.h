#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;
	const int MAX_POINT_LIGHT = 128;
	const int MAX_SPOT_LIGHT = 128;

	//�f�B���N�V�������C�g�\����
	struct DirectionLight
	{
		Vector3 direction = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 color = Vector3::Zero;
		float pad1 = 0.0f;
	};

	//�|�C���g���C�g�\����
	struct PointLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
	};

	//�X�|�b�g���C�g�\����
	struct SpotLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
		Vector3 direction = Vector3::Zero;
		float angle = 0.0f;
	};


	//���C�g�\����
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//�f�B���N�V�������C�g�̔z��
		Vector3 eyePos = Vector3::Zero;						//�J�����̈ʒu
		float pad = 0.0f;
		PointLight pointLight[MAX_POINT_LIGHT];				//�|�C���g���C�g�̔z��
		SpotLight spotLight[MAX_SPOT_LIGHT];				//�X�|�b�g���C�g�̔z��
	};


	class SceneLight : public Noncopyable
	{
	public:
		//�R���X�g���N�^
		SceneLight();

		//�f�B���N�V�������C�g�̐ݒ�
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].direction = direction;
			m_light.directionLight[lightNum].color = color;
		}

		//�|�C���g���C�g�̐ݒ�
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_light.pointLight[lightNum].position = position;
			m_light.pointLight[lightNum].color = color;
			m_light.pointLight[lightNum].range = range;
		}

		//�|�C���g���C�g�̃|�W�V�����ݒ�
		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_light.pointLight[lightNum].position = position;
		}

		//�X�|�b�g���C�g�̐ݒ�
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_light.spotLight[lightNum].position = position;
			m_light.spotLight[lightNum].color = color;
			m_light.spotLight[lightNum].range = range;
			m_light.spotLight[lightNum].direction = direction;
			m_light.spotLight[lightNum].angle = angle;
		}

		//�\���̂�Ԃ�
		Light& GetLight()
		{
			return m_light;
		}


	private:
		Light m_light;
	};

}
