#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"
namespace nsK2EngineLow {

	void PostEffect::Init(RenderTarget& renderTarget)
	{
		// 輝度抽出用のレンダリングターゲットを作成
		luminanceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),    // テクスチャの幅
			g_graphicsEngine->GetFrameBufferHeight(),   // テクスチャの高さ
			1,
			1,			
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// 輝度抽出用のスプライトを初期化
		// 初期化情報を作成する
		SpriteInitData luminanceSpriteInitData;
		// 輝度抽出用のシェーダーのファイルパスを指定する
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		// 頂点シェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		// ピクセルシェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		// スプライトの幅と高さはluminnceRenderTargetと同じ
		luminanceSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		luminanceSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// テクスチャはメインレンダリングターゲットのカラーバッファー
		luminanceSpriteInitData.m_textures[0] = &renderTarget.GetRenderTargetTexture();
		// 描き込むレンダリングターゲットのフォーマットを指定する
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// 作成した初期化情報をもとにスプライトを初期化する
		m_luminanceSprite.Init(luminanceSpriteInitData);

		// ガウシアンブラーを初期化
		gaussianBlur[0].Init(&luminanceRenderTarget.GetRenderTargetTexture());
		gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
		gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
		gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

		// ボケ画像を合成して書き込むためのスプライトを初期化
		SpriteInitData finalSpriteInitData;
		// ボケテクスチャを4枚指定する
		finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();
		// 解像度はmainRenderTargetの幅と高さ
		finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
		// 2D用のシェーダーを使用する
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
		// ただし、加算合成で描画するので、アルファブレンディングモードを加算にする
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		// カラーバッファーのフォーマットは例によって、32ビット浮動小数点バッファー
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// 初期化情報を元に加算合成用のスプライトを初期化する
		m_finalSprite.Init(finalSpriteInitData);

	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& renderTarget)
	{
		// 輝度抽出用ターゲットに変更し使えるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
		// ターゲットセット
		rc.SetRenderTargetAndViewport(luminanceRenderTarget);
		// ターゲットのクリア
		rc.ClearRenderTargetView(luminanceRenderTarget);
		//描画して終わるまで待つ
		m_luminanceSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);

		//ガウシアンブラーの実行(ぼかす)
		gaussianBlur[0].ExecuteOnGPU(rc, 10);
		gaussianBlur[1].ExecuteOnGPU(rc, 10);
		gaussianBlur[2].ExecuteOnGPU(rc, 10);
		gaussianBlur[3].ExecuteOnGPU(rc, 10);

		// MainRenderTargetセット
		rc.WaitUntilToPossibleSetRenderTarget(renderTarget);
		rc.SetRenderTargetAndViewport(renderTarget);
		//ボケ画像をメインレンダリングターゲットに加算合成(mainrenderTargetとブルームを合体)
		m_finalSprite.Draw(rc);
		// 終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(renderTarget);
	}
}
