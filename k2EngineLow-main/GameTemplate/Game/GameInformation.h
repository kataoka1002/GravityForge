#pragma once
class GameInformation : public IGameObject
{
public:
	~GameInformation();
	bool Start();
	void Update();
	void DeleteBossBGM();

	/// <summary>
	/// 倒した数を増やす
	/// </summary>
	void AddDefeatEnemyNum()
	{
		m_defeatEnemyNum++;
	}

	/// <summary>
	/// ボス戦に行くことが出来る
	/// </summary>
	void EnableBossBattle()
	{
		m_canGoBossBattle = true;
	}

	/// <summary>
	/// ボス戦中にする
	/// </summary>
	void StartBossBattle()
	{
		m_isInBossBattle = true;
	}

	/// <summary>
	/// ムービー中かどうかをセットする
	/// </summary>
	/// <param name="now"></param>
	void SetInMovie(bool now)
	{
		m_isInMovie = now;
	}

	/// <summary>
	/// オープニング中かどうかをセットする
	/// </summary>
	/// <param name="now"></param>
	void SetInOpening(bool now)
	{
		m_isInOpening = now;
	}

	/// <summary>
	/// ゲーム中かどうかをセットする
	/// </summary>
	/// <param name="now"></param>
	void SetInGame(bool now)
	{
		m_isInGame = now;
	}

	/// <summary>
	/// ボス戦に行くことができるかどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetCanGoBossBattle() const
	{
		return m_canGoBossBattle;
	}

	/// <summary>
	/// ボス戦中かどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetIsInBossBattle() const
	{
		return m_isInBossBattle;
	}

	/// <summary>
	/// ムービー中かどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetIsInMovie() const
	{
		return m_isInMovie;
	}

	/// <summary>
	/// オープニング中かどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetIsInOpening() const
	{
		return m_isInOpening;
	}

	/// <summary>
	/// ゲーム中かどうかを返す
	/// </summary>
	/// <returns></returns>
	const bool GetIsInGame() const
	{
		return m_isInGame;
	}

	/// <summary>
	/// ボス討伐時間を返す
	/// </summary>
	/// <returns></returns>
	const float GetBossKillTime()const
	{
		return m_bossKillTime;
	}

	/// <summary>
	/// 倒した敵の数を返す
	/// </summary>
	/// <returns></returns>
	const int GetKillEnemyNum()const
	{
		return m_defeatEnemyNum;
	}

private:
	int m_defeatEnemyNum = 0;			//倒した敵の数
	float m_bossKillTime = 0.0f;		//ボス撃破時間
	bool m_canGoBossBattle = false;		//ボス戦に行けるかどうか
	bool m_isInBossBattle = false;		//ボス戦中かどうか
	bool m_isInMovie = false;			//ムービー中かどうか
	bool m_isInOpening = true;			//オープニング中かどうか
	bool m_isInGame = false;			//メインゲームかどうか

	SoundSource* m_gameBGM = nullptr;	//ゲームのBGM
	SoundSource* m_bossBGM = nullptr;	//ボスのBGM
};