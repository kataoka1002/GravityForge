#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }
class GameInformation;
namespace nsBoss { class Boss; }

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
	void BossMovieProcess();

private:
	Vector3	m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル
	SpringCamera m_springCamera;			//ばねカメラ
	bool m_isNearCamera = false;			//近距離カメラかどうか
	float m_bossMakeTime = 0.7f;

	nsPlayer::Player* m_player = nullptr;	//プレイヤー
	nsBoss::Boss* m_boss = nullptr;			//ボス
	GameInformation* m_gameInfo = nullptr;
};

