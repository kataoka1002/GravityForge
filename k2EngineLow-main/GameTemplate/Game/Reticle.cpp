#include "stdafx.h"
#include "Reticle.h"
#include "Player.h"
#include "GameInformation.h"

namespace
{
	//ドットレティクルの大きさ
	const Vector3 DOT_RETICLE_SCALE = { 0.8f, 0.8f, 1.0f };

	//サイドレティクルの大きさ(最大値)
	const Vector3 MAX_SIDE_RETICLE_SCALE = { 1.4f, 1.4f, 1.0f };

	//サイドレティクルの大きさ(最小値)
	const Vector3 MIN_SIDE_RETICLE_SCALE = { 0.0f, 0.0f, 1.0f };

	//サイドレティクルの大きさ増加量
	const Vector3 ADD_SIDE_RETICLE_SCALE = { 0.15f, 0.15f, 0.0f };
}

namespace nsPlayer
{

	Reticle::Reticle()
	{

	}

	Reticle::~Reticle()
	{

	}

	bool Reticle::Start()
	{
		m_player = FindGO<Player>("player");

		m_gameInfo = FindGO<GameInformation>("gameinformation");

		m_dotReticle.Init("Assets/spriteData/dotReticle.dds", 1600.0f, 900.0f);
		m_dotReticle.SetScale(DOT_RETICLE_SCALE);
		m_dotReticle.Update();

		m_sideReticle.Init("Assets/spriteData/sideReticle.dds", 1600.0f, 900.0f);
		m_sideReticle.SetScale(MIN_SIDE_RETICLE_SCALE);
		m_sideReticle.Update();

		return true;
	}

	void Reticle::Update()
	{
		//大きさの計算
		CalcScale();

		//更新
		m_dotReticle.Update();
		m_sideReticle.Update();
	}

	void Reticle::CalcScale()
	{
		//プレイヤーがオブジェクトを保持しているなら
		if (m_player->GetIsHoldingObject())
		{
			//大きくする
			m_sideReticleScale += ADD_SIDE_RETICLE_SCALE;

			//一定の大きさで固定
			if (m_sideReticleScale.x >= 1.5f)
			{
				m_sideReticleScale = MAX_SIDE_RETICLE_SCALE;
			}
		}
		else
		{
			//小さくする
			m_sideReticleScale -= ADD_SIDE_RETICLE_SCALE;

			//一定の大きさで固定
			if (m_sideReticleScale.x <= 0.0f)
			{
				m_sideReticleScale = MIN_SIDE_RETICLE_SCALE;
			}
		}

		//セット
		m_sideReticle.SetScale(m_sideReticleScale);
	}

	void Reticle::Render(RenderContext& rc)
	{
		if (m_gameInfo->GetIsInGame() == false)
		{
			return;
		}

		m_dotReticle.Draw(rc);
		m_sideReticle.Draw(rc);
	}
}