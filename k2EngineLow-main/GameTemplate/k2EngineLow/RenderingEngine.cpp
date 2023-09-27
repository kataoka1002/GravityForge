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
		Init2DSprite();
		InitCopyToframeBufferSprite();
		InitGBuffer();
		InitShadow();
		InitDefferedLightingSprite();
		m_postEffect.Init(m_mainRenderTarget);
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

	void RenderingEngine::InitShadow()
	{
		m_shadowMapRender.Init();
	}

	void RenderingEngine::Init2DSprite()
	{
		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

		//2D用のターゲットの初期化
		m_2DRenderTarget.Create(
			m_mainRenderTarget.GetWidth(),
			m_mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

		//テクスチャは2Dレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		//解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		m_2DSprite.Init(spriteInitData);

		//テクスチャはメインレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度は2Dレンダ―ターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		m_mainSprite.Init(spriteInitData);
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

		//法線とスペキュラ用のターゲットを作成
		m_gBuffer[enGBufferNormal].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//ワールド座標用のターゲットを作成
		m_gBuffer[enGBufferWorldPos].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//カメラ空間の法線用のターゲットを作成
		m_gBuffer[enGBufferNormalInView].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//メタリックスムースマップ用のターゲットを作成
		m_gBuffer[enGBufferMetallicSmooth].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
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
		spriteInitData.m_textures[enGBufferNormal] = &m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferWorldPos] = &m_gBuffer[enGBufferWorldPos].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormalInView] = &m_gBuffer[enGBufferNormalInView].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferMetallicSmooth] = &m_gBuffer[enGBufferMetallicSmooth].GetRenderTargetTexture();
		int texNum = 5;
		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			spriteInitData.m_textures[texNum++] = &m_shadowMapRender.GetShadowMap(areaNo);
		}
		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &GetLightCB();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetLightCB());
		
		// ディファードレンダリング用のスプライトを初期化
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//GBufferへのレンダリング
		RenderToGBuffer(rc);

		//シャドウマップに影を描画
		DrawShadowMap(rc);

		//ディファードライティング
		DeferredLighting(rc);

		//ポストエフェクトの実行
		m_postEffect.Render(rc, m_mainRenderTarget);

		//フォントとスプライトの描画
		SpriteFontDraw(rc);

		//メインレンダリングターゲットの絵をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		//描画したオブジェクトをクリアする
		ObjectClear();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");

		// レンダリングターゲットをG-Bufferに変更して書き込む
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedo],   // 0番目のレンダリングターゲット
			&m_gBuffer[enGBufferNormal],
			&m_gBuffer[enGBufferWorldPos],
			&m_gBuffer[enGBufferNormalInView],
			&m_gBuffer[enGBufferMetallicSmooth],
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// まとめてモデルレンダーを描画
		for (auto& ModelData : ModelRenderObject) 
		{
			ModelData->OnDraw(rc);
		}

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

		EndGPUEvent();
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		BeginGPUEvent("DeferrdLighting");

		// レンダリング先をメインレンダリングターゲットにする
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// G-Bufferの内容を元にしてディファードライティング
		m_diferredLightingSprite.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::DrawShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("CascadeShadowDraw");

		//ライトカメラの方向を設定
		Vector3 lightCameraDir = { 1.0f, -1.0f, -1.0f };
		lightCameraDir.Normalize();

		//シャドウマップの処理
		m_shadowMapRender.Render(rc, lightCameraDir);

		EndGPUEvent();
	}

	void RenderingEngine::SpriteFontDraw(RenderContext& rc)
	{
		BeginGPUEvent("SpriteFontDraw");

		// 2D用のターゲットが使えるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// ターゲットセット
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// ターゲットのクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		// スプライトと文字を描画
		for(auto SpriteData : SpriteRenderObject)
		{
			SpriteData->OnDraw(rc);
		}
		for(auto FontData : FontRenderObject) 
		{
			FontData->OnDraw(rc);
		}

		// 描画されるまで待つ
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		// ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		// フレームバッファにセット
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToframeBufferSprite.Draw(rc);

		EndGPUEvent();
	}

	void RenderingEngine::ObjectClear()
	{
		//クリア
		ModelRenderObject.clear();
		SpriteRenderObject.clear();
		FontRenderObject.clear();
	}
}
