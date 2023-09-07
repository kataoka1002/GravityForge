#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		Init();
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	bool RenderingEngine::Start()
	{
		return true;
	}

	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitCopyToframeBufferSprite();
		InitGBuffer();
		InitDefferedLightingSprite();
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲットの作成
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),    
			g_graphicsEngine->GetFrameBufferHeight(),   
			1,
			1,											
			DXGI_FORMAT_R32G32B32A32_FLOAT,				
			DXGI_FORMAT_D32_FLOAT						
		);
	}

	void RenderingEngine::InitCopyToframeBufferSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToframeBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitGBuffer()
	{
		// アルベドカラー用のターゲットを作成
		float clearColor[] = { 0.5f,0.5f,0.5f,1.0f };
		m_gBuffer[enGBufferAlbedo].Create(
			g_graphicsEngine->GetFrameBufferWidth(),    
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void RenderingEngine::InitDefferedLightingSprite()
	{
		// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
		SpriteInitData spriteInitData;
		spriteInitData.m_width = FRAME_BUFFER_W;
		spriteInitData.m_height = FRAME_BUFFER_H;

		// ディファードライティングで使用するテクスチャを設定
		spriteInitData.m_textures[enGBufferAlbedo] = &m_gBuffer[enGBufferAlbedo].GetRenderTargetTexture();
		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";
		
		// ディファードレンダリング用のスプライトを初期化
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//GBufferへのレンダリング
		RenderToGBuffer(rc);

		//ディファードライティング
		DeferredLighting(rc);

		//メインレンダリングターゲットの絵をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		//描画したオブジェクトをクリアする
		ObjectClear();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		// レンダリングターゲットをG-Bufferに変更して書き込む
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedo]   // 0番目のレンダリングターゲット
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// まとめてモデルレンダーを描画
		for (auto& MobjData : ModelRenderObject) 
		{
			MobjData->OnDraw(rc);
		}

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		// レンダリング先をメインレンダリングターゲットにする
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// G-Bufferの内容を元にしてディファードライティング
		m_diferredLightingSprite.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		// フレームバッファにセット
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToframeBufferSprite.Draw(rc);
	}

	void RenderingEngine::ObjectClear()
	{
		//クリア
		ModelRenderObject.clear();
	}
}
