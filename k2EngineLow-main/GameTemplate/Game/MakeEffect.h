#pragma once

/// <summary>
/// �G�t�F�N�g�̖��O
/// </summary>
enum EffectName
{
	enEffectName_ObjectPush,
	enEffectName_ObjectSmoke,
	enEffectName_ObjectDrop,
	enEffectName_ObjectBom,
	enEffectName_ObjectSand,
	enEffectName_HumanAttack,
	enEffectName_PlayerPunch,
	enEffectName_BossMagic,
	enEffectName_BossPanch,
	enEffectName_BossSwipe,
	enEffectName_BossJumpAttack,
	enEffectName_EnemyBlood,
	enEffectName_BombLightning,
	enEffectName_Num,
};

class MakeEffect : public IGameObject
{
public:
	~MakeEffect();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

};

