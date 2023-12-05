#pragma once
#include "SkyCube.h"
class GameInformation;

class ResultUI : public IGameObject
{
public:
	enum enResultUIState
	{
		enResultUIState_Fade,
		enResultUIState_You,
		enResultUIState_Enemy,
		enResultUIState_EnemyNum,
		enResultUIState_Boss,
		enResultUIState_BossTime,
		enResultUIState_Score,
		enResultUIState_ScoreValue,
		enResultUIState_GoTitle,
	};

public:
	~ResultUI();
	bool Start();
	void Update();
	float CalcSinValue(float speed);
	void FadeProcess();
	void YouProcess();
	void EnemyProcess();
	void EnemyNumProcess();
	void BossProcess();
	void BossTimeProcess();
	void ScoreProcess();
	void ScoreValueProcess();
	void GoTitleProcess();
	void CalcScaleAndAlpha(float& scale, float& alpa);
	void CalcFontScale(float& scale, enResultUIState next);
	void Render(RenderContext& rc);

	void SetPlayerDead(bool which)
	{
		m_isPlayerDead = which;
	}

	void SetKillEnemyNum(const int num)
	{
		m_killEnemyNum = num;
	}

	void SetBossKillTime(const float time)
	{
		m_bossKillTime = time;
	}

	void SetFontAlpha()
	{
		m_fontColor -= 0.1f;
		m_enemyNumFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
		m_bossTimeFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
		m_scoreFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
	}

private:
	GameInformation* m_gameInfo = nullptr;
	SkyCube* m_skyCube = nullptr;

	enResultUIState m_resultUIState = enResultUIState_Fade;

	SpriteRender m_fadeSprite;
	SpriteRender m_youSprite;
	SpriteRender m_resultSprite;
	SpriteRender m_scoreSprite;
	SpriteRender m_goTitleSprite;
	SpriteRender m_enemySprite;
	SpriteRender m_bossSprite;

	float m_step = 0.1f;				//大きさが変わる速さ
	FontRender m_enemyNumFont;			//倒した敵の数のフォント
	int m_killEnemyNum = 0;				//倒した敵の数
	int m_drawNum = 0;					//今表示している,倒した敵の数
	float m_enemyFontScale = 1.0f;		//倒した敵の数のフォントの大きさ

	FontRender m_bossTimeFont;			//ボス撃破にかかった時間のフォント
	float m_bossKillTime = 0.0f;		//ボス撃破時間
	float m_bossFontScale = 1.0f;		//ボス撃破時間の大きさ
	float m_drawTime = 0.0f;			//今表示している,ボス撃破時間

	FontRender m_scoreFont;
	float m_score = 0.0f;				//スコア
	float m_scoreFontScale = 1.0f;		//スコアフォントの大きさ
	int m_drawScore = 0.0f;				//今表示している,スコア

	bool m_isPlayerDead = false;		//プレイヤーが死んだかどうか

	float m_degree = 0.0f;				//透明度の変更用角度

	float m_youAlpha = 0.0f;
	float m_youScale = 3.0f;
	float m_enemyAlpha = 0.0f;
	float m_enemyScale = 3.0f;
	float m_bossAlpha = 0.0f;
	float m_bossScale = 3.0f;
	float m_scoreAlpha = 0.0f;
	float m_scoreScale = 3.0f;
	float m_fadeAlpha = 1.0f;

	float m_time = 0.5f;				//最初処理が始まるまでの時間

	float m_fontColor = 1.0f;			//フォントの色、透明度
};

