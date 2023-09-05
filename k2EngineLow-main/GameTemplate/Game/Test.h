#pragma once
class Test : public IGameObject
{
public:
	Test();
	~Test();

	void Update();

	void Draw(RenderContext& rc);

private:
	ModelRender m_model;
};

