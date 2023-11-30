#pragma once
class GameInformation : public IGameObject
{
public:
	~GameInformation();
	bool Start();
	void Update();

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

private:
	int m_defeatEnemyNum = 0;			//倒した敵の数
	bool m_canGoBossBattle = false;		//ボス戦に行けるかどうか
	bool m_isInBossBattle = false;		//ボス戦中かどうか
	bool m_isInMovie = false;			//ムービー中かどうか
};