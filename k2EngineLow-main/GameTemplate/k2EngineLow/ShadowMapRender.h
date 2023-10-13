#pragma once
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	namespace shadow
	{
		class ShadowMapRender : public Noncopyable
		{
		public:
			/// <summary>
			/// 初期化
			/// </summary>
			void Init();

			/// <summary>
			/// 描画処理
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc, Vector3& lightDirection);

			/// <summary>
			/// シャドウマップを取得
			/// </summary>
			/// <param name="areaNo">エリア番号(近:0 , 中:1 , 遠:2)</param>
			/// <returns></returns>
			Texture& GetShadowMap(int areaNo)
			{
				return m_shadowMaps[areaNo].GetRenderTargetTexture();
			}

			/// <summary>
			/// ぼかしたシャドウマップを取得
			/// </summary>
			/// <param name="areaNo">エリア番号(近:0 , 中:1 , 遠:2)</param>
			/// <returns></returns>
			Texture& GetBokeShadowMap(int areaNo)
			{
				return m_blurShadowMap[areaNo].GetBokeTexture();
			}

			/// <summary>
			/// シャドウマップに描画するモデルのコンテナに追加
			/// </summary>
			/// <param name="model0">近景用のシャドウマップに描画するモデル</param>
			/// <param name="model1">中景用のシャドウマップ1に描画するモデル</param>
			/// <param name="model2">遠景用のシャドウマップ2に描画するモデル</param>
			void Add3DModel(Model& model0, Model& model1, Model& model2)
			{
				m_modelsArray[0].push_back(&model0);
				m_modelsArray[1].push_back(&model1);
				m_modelsArray[2].push_back(&model2);
			}

			/// <summary>
			/// ライトビュープロジェクション行列を取得
			/// </summary>
			/// <param name="areaNo">エリア番号(近:0 , 中:1 , 遠:2)</param>
			/// <returns></returns>
			const Matrix& GetLVPMatrix(int areaNo) const
			{
				return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
			}

		private:
			CascadeShadowMapMatrix m_cascadeShadowMapMatrix;    // カスケードシャドウマップの行列を扱うオブジェクト
			RenderTarget m_shadowMaps[NUM_SHADOW_MAP];          // シャドウマップ
			std::vector<Model*> m_modelsArray[NUM_SHADOW_MAP];  // シャドウマップに描画するモデルの配列
			GaussianBlur m_blurShadowMap[NUM_SHADOW_MAP];       // シャドウマップにブラーをかける処理
		};
	}
}

