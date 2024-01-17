#pragma once

/// <summary>
/// サウンドの名前
/// </summary>
enum SoundName
{
	enSoundName_TitleBGM,
	enSoundName_TitleClick,
	enSoundName_TitleCancel,
	enSoundName_TitleMove,
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

