#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	namespace shadow
	{
		void ShadowMapRender::Init()
		{
            float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            //近景用のシャドウマップ
            m_shadowMaps[0].Create(
                6096,
                6096,
                1,
                1,
                DXGI_FORMAT_R32G32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );

            //中景用のシャドウマップ
            m_shadowMaps[1].Create(
                2048,
                2048,
                1,
                1,
                DXGI_FORMAT_R32G32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );

            //遠景用のシャドウマップ
            m_shadowMaps[2].Create(
                1024,
                1024,
                1,
                1,
                DXGI_FORMAT_R32G32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );

            // ソフトシャドウを行うためのスプライト
            m_blurShadowMap[0].Init(&m_shadowMaps[0].GetRenderTargetTexture());
            m_blurShadowMap[1].Init(&m_shadowMaps[1].GetRenderTargetTexture());
            m_blurShadowMap[2].Init(&m_shadowMaps[2].GetRenderTargetTexture());
		}

		void ShadowMapRender::Render(RenderContext& rc, Vector3& lightDirection)
		{
            // ライトビュープロジェクションクロップ行列を計算
            m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(lightDirection);

            for (int i = 0; i < NUM_SHADOW_MAP; i++)
            {
                // 行列を定数バッファにセットする
                g_renderingEngine->SetLVP(GetLVPMatrix(i), i);
            }

            BeginGPUEvent("DrawShadowMap");

            int shadowMapNo = 0;
            for (auto& shadowMap : m_shadowMaps)
            {
                // ターゲットをシャドウマップにして書き込む
                rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
                rc.SetRenderTargetAndViewport(shadowMap);
                rc.ClearRenderTargetView(shadowMap);

                for (auto& model : m_modelsArray[shadowMapNo])
                {
                    model->Draw(
                        rc,
                        g_matIdentity,
                        m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
                    );
                }

                // 描画が終わったらクリア
                m_modelsArray[shadowMapNo].clear();

                // 書き込み完了待ち
                rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
                shadowMapNo++;
            }

            EndGPUEvent();


            BeginGPUEvent("BlurShadowMap");

            // ブラーを実行する。
            for (auto& blur : m_blurShadowMap)
            {
                blur.ExecuteOnGPU(rc, 1.0f);
            }

            EndGPUEvent();

		}
	}
}