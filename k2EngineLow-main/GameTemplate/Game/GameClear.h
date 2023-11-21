#pragma once
#include "Result.h"

class GameClear : public Result
{
public:
	GameClear();
	~GameClear();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//モデルの初期化
	void OnDestroy()				override;

private:

};

