#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;
	const int MAX_POINT_LIGHT = 128;

	//ディレクションライト構造体
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;
		float pad1 = 0.0f;
	};

	struct PointLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.01f;
	};

	//ライト構造体
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//ディレクションライトの配列
		Vector3 eyePos = Vector3::Zero;						//カメラの位置
		float pad = 0.0f;
		PointLight pointLight[MAX_POINT_LIGHT];				//ポイントライトの配列
	};


	class SceneLight : public Noncopyable
	{
	public:
		//コンストラクタ
		SceneLight();

		//ディレクションライトの設定
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].ligDirection = direction;
			m_light.directionLight[lightNum].ligColor = color;
		}

		//ポイントライトの設定
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_light.pointLight[lightNum].position = position;
			m_light.pointLight[lightNum].color = color;
			m_light.pointLight[lightNum].range = range;
		}

		//ポイントライトのポジション設定
		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_light.pointLight[lightNum].position = position;
		}

		//構造体を返す
		Light& GetLight()
		{
			return m_light;
		}


	private:
		Light m_light;
	};

}

