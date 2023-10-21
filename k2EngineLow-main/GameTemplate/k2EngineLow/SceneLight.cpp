#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow 
{
	SceneLight::SceneLight()
	{
		//カメラの位置の設定
		//m_light.eyePos = g_camera3D->GetPosition();
	}

	bool SceneLight::Start()
	{
		//カメラの位置の設定
		m_light.eyePos = g_camera3D->GetPosition();
		return true;
	}
}