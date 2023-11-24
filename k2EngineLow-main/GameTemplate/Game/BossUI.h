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

	private:
		SpriteRender m_HPFrame;		//HPのフレーム
		SpriteRender m_HPGage;		//HPのゲージ
		SpriteRender m_HPOrange;	//HPのオレンジゲージ
		SpriteRender m_HPBack;		//HPの背景
		SpriteRender m_name;		//ボスの名前

		Vector3 m_orangeScale = Vector3::One;	//オレンジゲージの大きさ

		Boss* m_boss = nullptr;
	};
}