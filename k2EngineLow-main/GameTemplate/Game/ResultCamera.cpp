#include "stdafx.h"
#include "ResultCamera.h"
#include "Result.h"

ResultCamera::ResultCamera()
{
	
}

ResultCamera::~ResultCamera()
{

}

bool ResultCamera::Start()
{
	return true;
}

void ResultCamera::SetTarget(const Vector3& targetPos)
{
	//ターゲットはモデルの右側
	target = targetPos;
	target.x += 15.0f;
	target.y += 42.0f;

	//注視点を設定する
	g_camera3D->SetTarget(target);

	//注視点から視点までのベクトルを求める
	Vector3 m_toCameraPos = { -15.0f,0.0f,30.0f };

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//視点を設定する。
	g_camera3D->SetPosition(pos);
}

void ResultCamera::Update()
{

}

