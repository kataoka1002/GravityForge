#pragma once
#include "ObjectBase.h"

class Teapot : public ObjectBase
{
public:

	Teapot();
	~Teapot();

	void Update();	//XVˆ—

	void Render(RenderContext& rc)	override;	//•`‰æˆ—
	void InitModel()				override;	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»

private:

};

