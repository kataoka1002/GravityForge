#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	void SetNearCamera(bool isNear);
	Vector3 CalcTargetPosition();

private:
	nsPlayer::Player* m_player = nullptr;				//プレイヤー
	Vector3	m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル
	SpringCamera m_springCamera;			//ばねカメラ

	bool m_isNearCamera = false;			//近距離カメラかどうか
};

