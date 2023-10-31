#pragma once

namespace nsPlayer
{

	//前方宣言
	class Player;

	class Reticle : public IGameObject
	{
	public:
		Reticle();
		~Reticle();
		bool Start();
		void Update();
		void Render(RenderContext& rc);
		void CalcScale();

	private:
		SpriteRender m_dotReticle;			//丸のレティクル
		SpriteRender m_sideReticle;			//周りのレティクル

		Vector3 m_dotReticleScale = Vector3::Zero;
		Vector3 m_sideReticleScale = Vector3::Zero;

		Player* m_player = nullptr;
	};

}