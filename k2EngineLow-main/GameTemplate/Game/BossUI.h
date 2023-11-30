#pragma once

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	class Boss;

	class BossUI : public IGameObject
	{
	public:
		~BossUI();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

		void DrawPlate(bool flag)
		{
			m_drawPlate = flag;
		}

		void DrawHP(bool flag)
		{
			m_drawHP = flag;
		}

	private:
		SpriteRender m_HPFrame;		//HPのフレーム
		SpriteRender m_HPGage;		//HPのゲージ
		SpriteRender m_HPOrange;	//HPのオレンジゲージ
		SpriteRender m_HPBack;		//HPの背景
		SpriteRender m_name;		//ボスの名前
		SpriteRender m_plate;		//プレート

		Vector3 m_orangeScale = Vector3::One;	//オレンジゲージの大きさ
		bool m_drawPlate = false;				//プレートを表示するかどうか
		bool m_drawHP = false;					//HPを表示するかどうか

		Boss* m_boss = nullptr;
	};
}