#pragma once
class ResultCamera;

class Result : public IGameObject, Noncopyable
{
public:
	//アニメーションクリップ
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Num
	};

public:
	Result() {}
	virtual ~Result() {}

	bool Start();
	void GoTitle();
	virtual void InitCamera() = 0;	//モデルの初期化
	virtual void OnDestroy() = 0;	//消えるときに呼ばれる処理

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

	Vector3				m_playerPosition = Vector3::Zero;
	ResultCamera*		m_resultCamera = nullptr;
};

