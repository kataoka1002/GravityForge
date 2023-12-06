#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }
namespace nsBoss { class Boss; }
class GameInformation;
class BlackFade;

class GameCamera : public IGameObject
{
public:
	enum enOPState
	{
		enOP_Camera1,
		enOP_Camera2,
		enOP_Player,
		enOP_Num
	};

public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	void SetNearCamera(bool isNear);
	Vector3 CalcTargetPosition();
	void BossMovieProcess();
	void OpeningProcess();
	void CalcOpeningFade(const enOPState& nextState, const Vector3& nextPos);
	void PlayVibration(int decline, int maxVib, bool& flag, int& vibration);
	void ManageVibration();
	void CalcChangeCamera();

	/// <summary>
	/// 揺れているかどうかのフラグをセットする
	/// </summary>
	/// <param name="flag">true：揺れている , false：揺れていない</param>
	void SetVibFlag(bool flag)
	{
		m_vibFlag = flag;
	}

	/// <summary>
	/// 近距離カメラかどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetIsNearCamera() const
	{
		return m_isNearCamera;
	}

private:
	enOPState m_opState = enOP_Camera1;		//オープニングステート
	Vector3	m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル
	SpringCamera m_springCamera;			//ばねカメラ
	bool m_isNearCamera = false;			//近距離カメラかどうか
	float m_bossMakeTime = 0.0f;			//ボスが出現するまでの時間
	Vector3 m_opTarget = Vector3::Zero;		//OPカメラのターゲット
	float m_opTime = 0.0f;					//オープニング用のタイマー
	bool m_camera1End = false;				//カメラ１が終わったかどうか
	int BGX = 0, BGY = 0;					//最終的な揺れの大きさ
	bool m_vibFlag = false;					//画面が揺れているかどうか
	float m_cameraChangeTime = 0.5f;		//近距離から遠距離に切り替わる時間
	bool m_isChangingCamera = false;		//近距離から遠距離に切り替わり中かどうか

	nsPlayer::Player* m_player = nullptr;	//プレイヤー
	nsBoss::Boss* m_boss = nullptr;			//ボス
	GameInformation* m_gameInfo = nullptr;
	BlackFade* m_blackFade = nullptr;
};

