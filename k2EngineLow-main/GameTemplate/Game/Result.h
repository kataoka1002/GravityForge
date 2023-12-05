#pragma once
class ResultCamera;
class BlackFade;

class Result : public IGameObject, Noncopyable
{
public:
	//アニメーションクリップ
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Dwarf,
		enAnimClip_Num
	};

public:
	Result() {}
	virtual ~Result() {}

	bool Start();
	void GoTitle();
	void SetAnimation();
	virtual void InitCamera() = 0;	//モデルの初期化
	virtual void OnDestroy() = 0;	//消えるときに呼ばれる処理
	virtual void InitUI() = 0;

	/// <summary>
	/// プレイヤーモデルのポジションを返す
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPlayerPosition() const
	{
		return m_playerPosition;
	}

protected:
	AnimationClip		animationClips[enAnimClip_Num];				// アニメーションクリップ
	EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// 現在設定されているアニメーションクリップ
	ModelRender			m_playerModel;								// プレイヤーモデル
	ModelRender			m_bgModel;									// 背景のモデル
	ResultCamera*		m_resultCamera = nullptr;					// カメラ
	BlackFade*			m_blackFade = nullptr;						// フェード

	Vector3				m_playerPosition = Vector3::Zero;			// プレイヤーのポジション
	float				m_time = 0.0;								// アニメーションを再生するまでの時間
	bool				m_didPlayAnim = false;						// アニメーション再生をしたかどうか
	bool				m_pressAButton = false;						// Aボタンを押したかどうか
};

