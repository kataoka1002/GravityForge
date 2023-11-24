#include "stdafx.h"
#include "BossUI.h"
#include "Boss.h"

namespace
{
	const Vector3 HP_FRAME_POSITION = { -449.0f,350.0f,0.0f };

	const Vector3 HP_GAGE_BACK_POSITION = { -434.0f,350.0f,0.0f };

	const Vector3 HP_NAME_POSITION = { 0.0f,400.0f,0.0f };

	const float HP_SCALE_Y = 0.8f;
}

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossUI::~BossUI()
	{

	}

	bool BossUI::Start()
	{
		m_boss = FindGO<Boss>("boss");

		m_HPFrame.Init("Assets/spriteData/UI/boss/bossFrame.dds", 898.0f, 76.0f * HP_SCALE_Y);
		m_HPFrame.SetPivot({ 0.0f,0.5f });
		m_HPFrame.SetPosition(HP_FRAME_POSITION);
		m_HPFrame.Update();
		m_HPGage.Init("Assets/spriteData/UI/boss/bossRed.dds", 868.0f, 45.0f * HP_SCALE_Y);
		m_HPGage.SetPivot({ 0.0f,0.5f });
		m_HPGage.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPGage.Update();
		m_HPBack.Init("Assets/spriteData/UI/boss/bossBlack.dds", 868.0f, 45.0f * HP_SCALE_Y);
		m_HPBack.SetPivot({ 0.0f,0.5f });
		m_HPBack.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPBack.Update();
		m_HPOrange.Init("Assets/spriteData/UI/boss/bossOrange.dds", 868.0f, 45.0f * HP_SCALE_Y);
		m_HPOrange.SetPivot({ 0.0f,0.5f });
		m_HPOrange.SetPosition(HP_GAGE_BACK_POSITION);
		m_HPOrange.Update();
		m_name.Init("Assets/spriteData/UI/boss/Doom.dds", 160.0f, 57.0f);
		m_name.SetPosition(HP_NAME_POSITION);
		m_name.Update();

		return true;
	}

	void BossUI::Update()
	{
		Vector3 HPGageScale = { m_boss->GetHPRatio(),1.0f,1.0f };

		//HPゲージの設定
		m_HPGage.SetScale(HPGageScale);
		m_HPGage.Update();

		//補間でゆっくりオレンジのサイズを小さくする
		m_orangeScale = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_orangeScale, HPGageScale);
		m_HPOrange.SetScale(m_orangeScale);
		m_HPOrange.Update();
	}

	void BossUI::Render(RenderContext& rc)
	{
		m_HPBack.Draw(rc);
		m_HPOrange.Draw(rc);
		m_HPGage.Draw(rc);
		m_HPFrame.Draw(rc);
		m_name.Draw(rc);
	}
}