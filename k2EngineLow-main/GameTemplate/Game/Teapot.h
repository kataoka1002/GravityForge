#pragma once
#include "ObjectBase.h"
class TeapotRender;

class Teapot : public ObjectBase
{
public:

	Teapot();
	~Teapot();

	void Update();				//更新処理
	void InitCarCollision();	//車のコリジョンの初期化

	void Render(RenderContext& rc)	override;	//描画処理
	void InitModel()				override;	//モデルの初期化
	void OnDestroy()				override;	//消えるときに呼ばれる処理
	void InitCollision()			override;	//コリジョンの初期化

private:
	TeapotRender*		m_teapotRender = nullptr;
	CollisionObject*	m_carCollision = nullptr;	// 車の形をしたコリジョン
};

