#pragma once

namespace nsK2EngineLow
{
	class ModelRender;

	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		bool Start();
		void Init();
		void InitMainRenderTarget();
		void InitCopyToframeBufferSprite();
		void InitGBuffer();
		void InitDefferedLightingSprite();
		void Execute(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		void ObjectClear();

		/// <summary>
		/// モデルレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="modelRender">モデルレンダー</param>
		void AddModelRenderObject(ModelRender* modelRender)
		{
			ModelRenderObject.push_back(modelRender);
		}


	private:

		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedo,           // アルベド
			enGBufferNum,              // G-Bufferの数
		};

		RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット
		RenderTarget m_gBuffer[enGBufferNum];			//GBuffer

		Sprite m_copyToframeBufferSprite;				//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;				//ディファードライティング用のスプライト

		std::vector<ModelRender*> ModelRenderObject;	//モデルレンダー

	};

}

