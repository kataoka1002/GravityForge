#include "stdafx.h"
#include "ResultUI.h"
#include "GameInformation.h"

namespace
{
	const int KILL_ENEMY_UP_SPEED = 1;

	const float KILL_BOSS_TIME_UP_SPEED = 2.0f;

	const int SCORE_UP_SPEED = 30;
}

ResultUI::~ResultUI()
{
	DeleteGO(m_gameInfo);
	DeleteGO(m_skyCube);
}

bool ResultUI::Start()
{
	m_gameInfo = FindGO<GameInformation>("gameinformation");
	m_skyCube = FindGO<SkyCube>("skycube");

	//死んだなら
	if (m_isPlayerDead == true)
	{
		m_youSprite.Init("Assets/spriteData/Result/YOUDEAD.dds", 723.0f, 142.0f);
		m_youSprite.SetPosition({ -200.0f,300.0f,0.0f });
	}
	else
	{
		m_youSprite.Init("Assets/spriteData/Result/YOUSURVIVE.dds", 966.0f, 142.0f);
		m_youSprite.SetPosition({ -200.0f,300.0f,0.0f });
	}
	m_youSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_youAlpha });
	m_youSprite.SetScale(m_youScale);
	m_youSprite.Update();

	m_resultSprite.Init("Assets/spriteData/Result/MatchResult.dds", 1600.0f, 418.0f);
	m_resultSprite.SetPosition({ 0.0f,-60.0f,0.0f });
	m_resultSprite.Update();

	m_scoreSprite.Init("Assets/spriteData/Result/TOTALSCORE.dds", 285.0f, 31.0f);
	m_scoreSprite.SetPosition({ -500.0f,-230.0f,0.0f });
	m_scoreSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_scoreAlpha });
	m_scoreSprite.SetScale(m_scoreScale);
	m_scoreSprite.Update();

	m_goTitleSprite.Init("Assets/spriteData/Result/GoTitle.dds", 341.0f, 72.0f);
	m_goTitleSprite.SetPosition({ -250.0f,-350.0f,0.0f });
	m_goTitleSprite.SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	m_goTitleSprite.SetScale(0.9f);
	m_goTitleSprite.Update();

	m_enemySprite.Init("Assets/spriteData/Result/killEnemyNum.dds", 247.0f, 33.0f);
	m_enemySprite.SetPosition({ -500.0f,20.0f,0.0f });
	m_enemySprite.SetMulColor({ 1.0f,1.0f,1.0f,m_enemyAlpha });
	m_enemySprite.SetScale(m_enemyScale);
	m_enemySprite.Update();

	m_bossSprite.Init("Assets/spriteData/Result/killBossTime.dds", 244.0f, 34.0f);
	m_bossSprite.SetPosition({ -500.0f,-30.0f,0.0f });
	m_bossSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_bossAlpha });
	m_bossSprite.SetScale(m_bossScale);
	m_bossSprite.Update();

	m_fadeSprite.Init("Assets/spriteData/title/NowLoading.dds", 1600.0f, 900.0f);
	m_fadeSprite.SetMulColor({ 0.0f,0.0f,0.0f,m_fadeAlpha });
	m_fadeSprite.Update();

	//倒した敵の数と撃破時間を取得
	m_killEnemyNum = m_gameInfo->GetKillEnemyNum();
	m_bossKillTime = m_gameInfo->GetBossKillTime();

	return true;
}

void ResultUI::Update()
{
	//ステートによって処理を変更
	switch (m_resultUIState)
	{
	case ResultUI::enResultUIState_Fade:
		FadeProcess();
		break;
	case ResultUI::enResultUIState_You:	
		YouProcess();		
		break;
	case ResultUI::enResultUIState_Enemy:
		EnemyProcess();
		break;
	case ResultUI::enResultUIState_EnemyNum:
		EnemyNumProcess();
		break;
	case ResultUI::enResultUIState_Boss:
		BossProcess();
		break;
	case ResultUI::enResultUIState_BossTime:
		BossTimeProcess();
		break;
	case ResultUI::enResultUIState_Score:
		ScoreProcess();
		break;
	case ResultUI::enResultUIState_ScoreValue:
		ScoreValueProcess();
		break;
	case ResultUI::enResultUIState_GoTitle:
		GoTitleProcess();
		break;
	default:
		break;
	}
}

void ResultUI::FadeProcess()
{
	//最初の数秒は処理しない
	m_time -= g_gameTime->GetFrameDeltaTime();
	if (m_time >= 0.0f)
	{
		return;
	}

	//フェードアウト
	m_fadeAlpha -= 0.1f;

	//フェードアウトが終わったら次のステートへ
	if (m_fadeAlpha <= 0.0f)
	{
		m_fadeAlpha = 0.0f;
		m_resultUIState = enResultUIState_You;
	}
	m_fadeSprite.SetMulColor({ 0.0f,0.0f,0.0f,m_fadeAlpha });
	m_fadeSprite.Update();
}

void ResultUI::YouProcess()
{
	//透明度と大きさを計算
	CalcScaleAndAlpha(m_youScale, m_youAlpha);

	//目的の大きさになったら次のステートへ
	if (m_youScale <= 1.0f)
	{
		m_resultUIState = enResultUIState_Enemy;
	}

	m_youSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_youAlpha });
	m_youSprite.SetScale(m_youScale);
	m_youSprite.Update();
}

void ResultUI::EnemyProcess()
{
	//透明度と大きさを計算
	CalcScaleAndAlpha(m_enemyScale, m_enemyAlpha);

	//目的の大きさになったら次のステートへ
	if (m_enemyScale <= 1.0f)
	{
		m_resultUIState = enResultUIState_EnemyNum;
	}

	m_enemySprite.SetMulColor({ 1.0f,1.0f,1.0f,m_enemyAlpha });
	m_enemySprite.SetScale(m_enemyScale);
	m_enemySprite.Update();
}

void ResultUI::EnemyNumProcess()
{
	//表示する値をだんだん大きくする
	if (m_drawNum < m_killEnemyNum)
	{
		m_drawNum += KILL_ENEMY_UP_SPEED;
	}
	else if (m_drawNum == m_killEnemyNum)
	{
		//大きくして小さくする(一定の大きさで次のステートへ)
		CalcFontScale(m_enemyFontScale, enResultUIState_Boss);

		m_enemyNumFont.SetScale(m_enemyFontScale);		
	}

	wchar_t text[256];
	swprintf_s(text, 256, L"x%02d", m_drawNum);
	m_enemyNumFont.SetText(text);
	m_enemyNumFont.SetPosition({ 0.0f,50.0f,0.0f });
}

void ResultUI::BossProcess()
{
	//透明度と大きさを計算
	CalcScaleAndAlpha(m_bossScale, m_bossAlpha);

	//目的の大きさになったら次のステートへ
	if (m_bossScale <= 1.0f)
	{
		m_resultUIState = enResultUIState_BossTime;
	}

	m_bossSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_bossAlpha });
	m_bossSprite.SetScale(m_bossScale);
	m_bossSprite.Update();
}

void ResultUI::BossTimeProcess()
{
	//表示する値をだんだん大きくする
	if (m_drawTime < m_bossKillTime)
	{
		m_drawTime += KILL_BOSS_TIME_UP_SPEED;
	}
	else if (m_drawTime >= m_bossKillTime)
	{
		//大きくして小さくする(一定の大きさで次のステートへ)
		CalcFontScale(m_bossFontScale, enResultUIState_Score);

		m_bossTimeFont.SetScale(m_bossFontScale);
	}

	wchar_t text[256];
	int minute = (int)m_drawTime / 60;
	int sec = (int)m_drawTime % 60;
	swprintf_s(text, 256, L"%02d:%02d", minute, sec);
	m_bossTimeFont.SetText(text);
	m_bossTimeFont.SetPosition({ 0.0f,0.0f,0.0f });
}

void ResultUI::ScoreProcess()
{
	//透明度と大きさを計算
	CalcScaleAndAlpha(m_scoreScale, m_scoreAlpha);

	//目的の大きさになったら次のステートへ
	if (m_scoreScale <= 1.0f)
	{
		m_resultUIState = enResultUIState_ScoreValue;
	}

	m_scoreSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_scoreAlpha });
	m_scoreSprite.SetScale(m_scoreScale);
	m_scoreSprite.Update();
}

void ResultUI::ScoreValueProcess()
{
	//表示する値をだんだん大きくする
	if (m_drawScore < m_score)
	{
		m_drawScore += SCORE_UP_SPEED;
	}
	else if (m_drawScore >= m_score)
	{
		m_drawScore = m_score;

		//大きくして小さくする(一定の大きさで次のステートへ)
		CalcFontScale(m_scoreFontScale, enResultUIState_GoTitle);

		m_scoreFont.SetScale(m_scoreFontScale);
	}

	wchar_t text[256];
	swprintf_s(text, 256, L"%d", m_drawScore);
	m_scoreFont.SetText(text);
	m_scoreFont.SetPosition({ 0.0f,-250.0f,0.0f });
}

void ResultUI::GoTitleProcess()
{
	//消えたり出たりする
	float fontAlpha = CalcSinValue(1.0f);

	m_goTitleSprite.SetMulColor({ 1.0f,1.0f,1.0f,fontAlpha });
	m_goTitleSprite.Update();
}

void ResultUI::CalcScaleAndAlpha(float& scale, float& alpa)
{
	//α値を大きくする
	alpa += 0.075f;
	if (alpa >= 1.0f)
	{
		alpa = 1.0f;
	}

	//大きさを小さくする
	scale -= 0.1f;
	if (scale <= 1.0f)
	{
		scale = 1.0f;
	}
}

void ResultUI::CalcFontScale(float& scale, enResultUIState next)
{
	//サイズの変化
	scale += m_step;

	//2.0に達したらステップを反転して減少方向に変更
	if (scale >= 2.0)
	{
		m_step *= -1.0f;
	}
	//1.0に戻ったらステップを再び増加方向に変更
	else if (scale < 1.0)
	{
		scale = 1.0f;
		m_step *= -1.0f;
		m_resultUIState = next;
	}
}

float ResultUI::CalcSinValue(float speed)
{
	//sin波を使って滑らかに上下させる
	//角度を大きくしていく
	m_degree += speed;

	//360度で-1～1を一周するので0度に戻す
	if (m_degree >= 180.0f)
	{
		m_degree = 0.0f;
	}

	//角度をラジアンに変換
	float rad = m_degree * 3.141592 / 180.0f;

	//sinの値を求め,それを透明度にする
	float sinValue = sin(rad);
	return sinValue;
}

void ResultUI::Render(RenderContext& rc)
{
	m_youSprite.Draw(rc);
	m_resultSprite.Draw(rc);
	m_scoreSprite.Draw(rc);
	m_goTitleSprite.Draw(rc);
	m_enemySprite.Draw(rc);
	m_bossSprite.Draw(rc);

	m_enemyNumFont.Draw(rc);
	m_bossTimeFont.Draw(rc);
	m_scoreFont.Draw(rc);

	m_fadeSprite.Draw(rc);
}