#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		//Init();
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
		InitZPrepassTarget();
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

	void RenderingEngine::InitZPrepassTarget()
	{
		float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
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

		//モデルの無い位置(スカイキューブの位置)のワールド座標を大きくすることで影を落とさない
		float clearWorld[] = { 10000.0f,10000.0f,10000.0f,1.0f };
		//ワールド座標用のターゲットを作成
		m_gBuffer[enGBufferWorldPos].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			clearWorld
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
		// GIテクスチャを作成するためのブラー処理を初期化する。
		m_giTextureBlur[eGITextureBlur_1024x1024].Init(&g_graphicsEngine->GetRaytracingOutputTexture(), 1024, 1024);
		m_giTextureBlur[eGITextureBlur_512x512].Init(&m_giTextureBlur[eGITextureBlur_1024x1024].GetBokeTexture(), 512, 512);
		m_giTextureBlur[eGITextureBlur_256x256].Init(&m_giTextureBlur[eGITextureBlur_512x512].GetBokeTexture(), 256, 256);
		m_giTextureBlur[eGITextureBlur_128x128].Init(&m_giTextureBlur[eGITextureBlur_256x256].GetBokeTexture(), 128, 128);

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
			spriteInitData.m_textures[texNum++] = &m_shadowMapRender.GetBokeShadowMap(areaNo);
		}
		//レイトレが可能なら
		if (g_graphicsEngine->IsPossibleRaytracing()) 
		{
			spriteInitData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetRaytracingOutputTexture();
			spriteInitData.m_expandShaderResoruceView[1] = &m_giTextureBlur[eGITextureBlur_1024x1024].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[2] = &m_giTextureBlur[eGITextureBlur_512x512].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[3] = &m_giTextureBlur[eGITextureBlur_256x256].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[4] = &m_giTextureBlur[eGITextureBlur_128x128].GetBokeTexture();
		}
		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";
		//spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBuffer = &m_sceneLight.GetLight();
		//spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);
		spriteInitData.m_expandConstantBufferSize = sizeof(m_sceneLight.GetLight());

		if (m_iblData.m_texture.IsValid()) 
		{
			spriteInitData.m_textures[texNum++] = &m_iblData.m_texture;
			m_deferredLightingCB.m_isIBL = 1;
			m_deferredLightingCB.m_iblLuminance = m_iblData.m_intencity;
		}
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// ディファードレンダリング用のスプライトを初期化
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::ReInitIBL(const wchar_t* iblTexFilePath, float luminance)
	{
		// IBLデータを初期化。
		InitIBLData(iblTexFilePath, luminance);

		InitDefferedLightingSprite();
	}

	void RenderingEngine::InitIBLData(const wchar_t* iblTexFilePath, float intencity)
	{
		m_iblData.m_texture.InitFromDDSFile(iblTexFilePath);
		m_iblData.m_intencity = intencity;
		g_graphicsEngine->SetRaytracingSkyCubeBox(m_iblData.m_texture);
	}

	void RenderingEngine::CalcViewProjectionMatrixForViewCulling()
	{
		Matrix projMatrix;
		projMatrix.MakeProjectionMatrix(
			g_camera3D->GetViewAngle() * 1.5f,
			g_camera3D->GetAspect(),
			g_camera3D->GetNear(),
			g_camera3D->GetFar()
		);
		m_viewProjMatrixForViewCulling.Multiply(g_camera3D->GetViewMatrix(), projMatrix);
	}

	void RenderingEngine::Update()
	{
		// ビューカリング用のビュープロジェクション行列の計算。
		CalcViewProjectionMatrixForViewCulling();
		// シーンのジオメトリ情報の更新。
		m_sceneGeometryData.Update();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// シーンライトのデータをコピー。
		m_deferredLightingCB.m_light = m_sceneLight.GetLight();
		m_deferredLightingCB.m_isEnableRaytracing = IsEnableRaytracing() ? 1 : 0;

		// ZPrepass
		ZPrepass(rc);

		//GBufferへのレンダリング
		RenderToGBuffer(rc);

		//エフェクトの描画
		DrawEffect(rc);

		//シャドウマップに影を描画
		DrawShadowMap(rc);

		//EyePosはカメラの位置
		SetEyePosition(g_camera3D->GetPosition());
		
		// レイトレ用のライトデータをコピー。
		//m_raytracingLightData.m_directionalLight = m_sceneLight.GetSceneLight().directionalLight[0];
		m_raytracingLightData.m_iblIntencity = m_iblData.m_intencity;
		m_raytracingLightData.m_ambientLight = Vector3{ 1.1f,1.1f,1.1f };
		m_raytracingLightData.m_enableIBLTexture = m_iblData.m_texture.IsValid() ? 1 : 0;

		// レイトレで映り込み画像を作成する。
		/*if (IsEnableRaytracing()) 
		{
			g_graphicsEngine->DispatchRaytracing(rc);

			for (auto& blur : m_giTextureBlur) 
			{
				blur.ExecuteOnGPU(rc, 5);
			}
		}*/

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

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");
		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : ModelRenderObject) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
		EndGPUEvent();
	}

	void RenderingEngine::DrawEffect(RenderContext& rc)
	{
		BeginGPUEvent("DrawEffect");

		// アルベドのgバッファーをセット
		rc.WaitUntilToPossibleSetRenderTarget(m_gBuffer[enGBufferAlbedo]);
		rc.SetRenderTargetAndViewport(m_gBuffer[enGBufferAlbedo]);
		
		//ここでエフェクトドロー。
		EffectEngine::GetInstance()->Draw();

		// 終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_gBuffer[enGBufferAlbedo]);
	
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
		BeginGPUEvent("CascadeShadow");

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
