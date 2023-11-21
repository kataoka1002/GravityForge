#pragma once
#include "Result.h"

class GameClear : public Result
{
public:
	GameClear();
	~GameClear();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	void OnDestroy()				override;

private:

};

