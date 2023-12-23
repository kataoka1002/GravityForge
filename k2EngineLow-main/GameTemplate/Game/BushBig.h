#pragma once
#include "ObjectBase.h"
class BushBigRender;

class BushBig : public ObjectBase
{
public:

	BushBig();
	~BushBig();

	void Update();	//更新処理

	void Render(RenderContext& rc)	override;	//描画処理
	void InitModel()				override;	//モデルの初期化
	void OnDestroy()				override;	//消えるときに呼ばれる処理
	void InitCollision()			override;	//コリジョンの初期化

private:
	BushBigRender* m_bushBigRender = nullptr;
};

