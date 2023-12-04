#pragma once
class GameInformation;

class MissionUI : public IGameObject
{
public:
	~MissionUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_missionSprite;
	SpriteRender m_missionZombie;
	SpriteRender m_missionBoss;

	GameInformation* m_gameInfo = nullptr;
};

