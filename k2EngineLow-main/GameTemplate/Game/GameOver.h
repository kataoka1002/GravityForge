#pragma once
#include "Result.h"

class GameOver : public Result
{
public:
	GameOver();
	~GameOver();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//モデルの初期化
	void OnDestroy()				override;

private:

};