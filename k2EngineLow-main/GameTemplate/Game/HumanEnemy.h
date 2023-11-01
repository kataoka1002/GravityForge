#pragma once
#include "EnemyBase.h"

class HumanEnemy : public EnemyBase
{
public:
	enum EnHumanEnemyState
	{
		enHumanEnemyState_Idle,		//待機中
		enHumanEnemyState_Num
	};

public:
	HumanEnemy();
	~HumanEnemy();

	void Update();								//更新処理
	void OnDestroy()				override;	//消えるときに呼ばれる処理
	void InitModel()				override;	//モデルの初期化
	void Render(RenderContext& rc)	override;	//描画処理

protected:
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];				// アニメーションクリップ
	EnHumanEnemyState	m_humanEnemyState = enHumanEnemyState_Idle;		// プレイヤーのステート(状態)を表す変数
};

