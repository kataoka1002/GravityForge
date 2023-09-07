#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;

	//ディレクションライト構造体
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;
		float pad1 = 0.0f;
	};

	//ライト構造体
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//ディレクションライトの配列
	};


	class SceneLight : public Noncopyable
	{
	public:
		//ディレクションライトの設定
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].ligDirection = direction;
			m_light.directionLight[lightNum].ligColor = color;
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

