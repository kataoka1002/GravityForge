#pragma once
#include "SceneLight.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	class ModelRender;
	class SpriteRender;
	class FontRender;

	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		bool Start();
		void Init();
		void InitMainRenderTarget();
		void Init2DSprite();
		void InitCopyToframeBufferSprite();
		void InitGBuffer();
		void InitDefferedLightingSprite();
		void Execute(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void SpriteFontDraw(RenderContext& rc);
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

		/// <summary>
		/// スプライトレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="spriteRender">スプライトレンダー</param>
		void AddSpriteRenderObject(SpriteRender* spriteRender)
		{
			SpriteRenderObject.push_back(spriteRender);
		}

		/// <summary>
		/// フォントレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="fontRender">フォントレンダー</param>
		void AddFontRenderObject(FontRender* fontRender)
		{
			FontRenderObject.push_back(fontRender);
		}

		/// <summary>
		/// シーンライトの構造体を返す
		/// </summary>
		/// <returns></returns>
		Light& GetLightCB()
		{
			return m_sceneLight.GetLight();
		}

		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="direction">方向</param>
		/// <param name="color">カラー</param>
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNum, direction, color);
		}

		/// <summary>
		/// ポイントライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="position">ポジション</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_sceneLight.SetPointLight(lightNum, position, color, range);
		}

		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_sceneLight.SetPointLightPosition(lightNum, position);
		}

		/// <summary>
		/// スポットライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="position">ポジション</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		/// <param name="direction">射出方向</param>
		/// <param name="angle">射出角度</param>
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_sceneLight.SetSpotLight(lightNum, position, color, range, direction, angle);
		}

		/// <summary>
		/// 半球ライトの設定
		/// </summary>
		/// <param name="skyColor">空の色</param>
		/// <param name="groundColor">地面の色</param>
		/// <param name="normal">地面の法線</param>
		void SetHemLight(Vector3 skyColor, Vector3 groundColor, Vector3 normal)
		{
			m_sceneLight.SetHemLight(skyColor, groundColor, normal);
		}


	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedo,				// アルベド
			enGBufferNormal,				// 法線
			enGBufferWorldPos,				// ワールド座標
			enGBufferNormalInView,			// カメラ空間の法線
			enGBufferMetallicSmooth,		// メタリックスムースマップ
			enGBufferNum,					// G-Bufferの数
		};

		SceneLight m_sceneLight;                        // シーンライト
		RenderTarget m_mainRenderTarget;				// メインレンダリングターゲット
		RenderTarget m_gBuffer[enGBufferNum];			// GBuffer
		RenderTarget m_2DRenderTarget;					// 2D描画用のターゲット
		Sprite m_copyToframeBufferSprite;				// メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;				// ディファードライティング用のスプライト
		Sprite m_2DSprite;								// 2D描画用のスプライト
		Sprite m_mainSprite;							// メインスプライト
		PostEffect m_postEffect;						// ポストエフェクト
		Camera m_lightCamera;

		std::vector<ModelRender*> ModelRenderObject;	// モデルレンダー
		std::vector<SpriteRender*> SpriteRenderObject;	// スプライトレンダー
		std::vector<FontRender*> FontRenderObject;		// フォントレンダー

	};

}

