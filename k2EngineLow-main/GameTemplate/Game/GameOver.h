#pragma once
#include "Result.h"
class ResultUI;

class GameOver : public Result
{
public:
	GameOver();
	~GameOver();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	void OnDestroy() override;
	void InitUI() override;

private:
	ResultUI* m_resultUI = nullptr;
};