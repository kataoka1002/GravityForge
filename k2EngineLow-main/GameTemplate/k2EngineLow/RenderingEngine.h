#pragma once
#include "SceneLight.h"

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


	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedo,    // アルベド
			enGBufferNormal,	//法線
			enGBufferWorldPos,	//ワールド座標
			enGBufferNum,       // G-Bufferの数
		};

		SceneLight m_sceneLight;                        // シーンライト
		RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット
		RenderTarget m_gBuffer[enGBufferNum];			//GBuffer
		Sprite m_copyToframeBufferSprite;				//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;				//ディファードライティング用のスプライト

		std::vector<ModelRender*> ModelRenderObject;	//モデルレンダー

	};

}

