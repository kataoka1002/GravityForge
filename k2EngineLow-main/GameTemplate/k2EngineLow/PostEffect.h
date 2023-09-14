#pragma once
namespace nsK2EngineLow
{
	class PostEffect : public Noncopyable
	{
	public:
		void Init(RenderTarget& renderTarget);
		void Render(RenderContext& rc, RenderTarget& renderTarget);

	private:
		RenderTarget	luminanceRenderTarget;
		Sprite			m_luminanceSprite;
		Sprite			m_finalSprite;
		GaussianBlur	gaussianBlur[4];
	};
}

