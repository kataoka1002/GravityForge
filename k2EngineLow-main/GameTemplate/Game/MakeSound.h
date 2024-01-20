#pragma once

/// <summary>
/// サウンドの名前
/// </summary>
enum SoundName
{
	enSoundName_GameBGM,
	enSoundName_BossBGM,
	enSoundName_ResultBGM,
	enSoundName_TitleBGM,
	enSoundName_TitleClick,
	enSoundName_TitleCancel,
	enSoundName_TitleMove,
	enSoundName_OpDistortion,
	enSoundName_ObjBomb,
	enSoundName_ObjBlowWind,
	enSoundName_ObjUpWind,
	enSoundName_BossName,
	enSoundName_BossJumpAttack,
	enSoundName_BossWalk,
	enSoundName_BossSlash,
	enSoundName_BossPunch,
	enSoundName_PlayerWalk,
	enSoundName_PlayerPunch,
	enSoundName_HumanEnemyAttack,
	enSoundName_HumanEnemyDead,
	enSoundName_HumanEnemyBatan,
	enSoundName_ZombieVoice,
	enSoundName_Num,
};

class MakeSound : public IGameObject
{
public:
	~MakeSound();
	bool Start();
	void Update();

private:

};

