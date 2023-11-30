#include "stdafx.h"
#include "PlayerUI.h"
#include "Player.h"
#include "GameInformation.h"

namespace
{
	const Vector3 HP_FRAME_POSITION = { -690.0f,-350.0f,0.0f };

	const Vector3 HP_GAGE_BACK_POSITION = { -680.0f,-350.0f,0.0f };

	const Vector3 HP_PLUS_POSITION = { -730.0f,-350.0f,0.0f };
}

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	PlayerUI::~PlayerUI()
	{

	}

	bool PlayerUI::Start()
	{
		m_player = FindGO<Player>("player");

		m_gameInfo = FindGO<GameInformation>("gameinformation");

		m_HPFrame.Init("Assets/spriteData/UI/HPFrame.dds", 439.0f, 46.0f);
		m_HPFrame.SetPivot({ 0.0f,0.5f });
		m_HPFrame.SetPosition(HP_FRAME_POSITION);
		m_HPFrame.Update();
		m_HPGage.Init("Assets/spriteData/UI/HPGage.dds", 423.0f, 30.0f);
		m_HPGage.SetPivot({ 0.0f,0.5f });
		m_HPGage.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPGage.Update();
		m_HPBack.Init("Assets/spriteData/UI/HPBack.dds", 423.0f, 30.0f);
		m_HPBack.SetPivot({ 0.0f,0.5f });
		m_HPBack.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPBack.Update();
		m_HPWhite.Init("Assets/spriteData/UI/HPGageWhite.dds", 423.0f, 30.0f);
		m_HPWhite.SetPivot({ 0.0f,0.5f });
		m_HPWhite.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPWhite.Update();
		m_plus.Init("Assets/spriteData/UI/+.dds", 40.0f, 40.0f);
		m_plus.SetPivot({ 0.0f,0.5f });
		m_plus.SetPosition(HP_PLUS_POSITION);
		m_plus.Update();

		return true;
	}

	void PlayerUI::Update()
	{
		Vector3 HPGageScale = { m_player->GetHPRatio(),1.0f,1.0f };

		//HPゲージの設定
		m_HPGage.SetScale(HPGageScale);
		m_HPGage.Update();

		//補間でゆっくり白のサイズを小さくする
		m_whiteScale = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_whiteScale, HPGageScale);
		m_HPWhite.SetScale(m_whiteScale);
		m_HPWhite.Update();
	}

	void PlayerUI::Render(RenderContext& rc)
	{
		if(m_gameInfo->GetIsInMovie())
		{
			return;
		}

		m_HPBack.Draw(rc);
		m_HPWhite.Draw(rc);
		m_HPGage.Draw(rc);
		m_HPFrame.Draw(rc);
		m_plus.Draw(rc);
	}
}