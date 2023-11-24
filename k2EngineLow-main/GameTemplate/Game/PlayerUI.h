#pragma once

/// <summary>
/// プレイヤーの名前空間
/// </summary>
namespace nsPlayer
{
	//前方宣言
	class Player;

	class PlayerUI : public IGameObject
	{
	public:
		~PlayerUI();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

	private:
		SpriteRender m_HPFrame;		//HPのフレーム
		SpriteRender m_HPGage;		//HPのゲージ
		SpriteRender m_HPWhite;		//HPの白ゲージ
		SpriteRender m_HPBack;		//HPの背景
		SpriteRender m_plus;		//HPの＋

		Vector3 m_whiteScale = Vector3::One;	//白ゲージの大きさ

		Player* m_player = nullptr;
	};
}