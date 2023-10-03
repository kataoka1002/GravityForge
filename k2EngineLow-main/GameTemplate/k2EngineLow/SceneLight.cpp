#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow 
{
	SceneLight::SceneLight()
	{
		//ƒJƒƒ‰‚ÌˆÊ’u‚ÌÝ’è
		m_light.eyePos = g_camera3D->GetPosition();
	}
}