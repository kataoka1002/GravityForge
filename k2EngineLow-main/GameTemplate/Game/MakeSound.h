#pragma once

/// <summary>
/// サウンドの名前
/// </summary>
enum SoundName
{
	enSoundName_TitleBGM,
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

