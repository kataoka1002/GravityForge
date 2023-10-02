#pragma once

#include "DescriptorHeap.h"
#include "RaytracingInstance.h"
#include "TLASBuffer.h"
#include "BLASBuffer.h"
#include "RaytracingPSO.h"
#include "RaytracingShaderTable.h"
#include "RaytracingDescriptorHeaps.h"

namespace nsK2EngineLow {
	class Model;
	namespace raytracing {

		/// <summary>
		/// 初期化情報
		/// </summary>
		struct InitData {
			void* m_expandShaderResource;			// 拡張シェーダーリソースの配列。
			int		m_expandShaderResourceSize;		// 拡張シェーダーリソースのサイズの配列。
			DXGI_FORMAT m_outputColorBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;	// レイトレの結果を出力するカラーバッファのフォーマット。
		};
		/// <summary>
		/// 拡張シェーダーリソース
		/// </summary>
		struct ExpanadSRV {
			void Init(void* srcData, int srcDataSize)
			{
				m_srcData = srcData;
				m_srcDataSize = srcDataSize;
				m_structuredBuffer.Init(srcDataSize, 1, srcData/*, false*/);
			}
			void* m_srcData = nullptr;				// ソースデータ。
			int m_srcDataSize = 0;					// ソースデータのサイズ。
			StructuredBuffer m_structuredBuffer;	// ストラクチャードバッファ。
		};
		using ExpanadSRVPtr = std::unique_ptr< ExpanadSRV>;

		class Engine : public Noncopyable
		{
		public:
			/// <summary>
			/// レイトレーシングをディスパッチ。
			/// </summary>
			/// <param name="rc">レンダリングコンテキスト</param>
			void Dispatch(RenderContext& rc);
			/// <summary>
			/// ジオメトリを登録。
			/// </summary>
			/// <param name="model">モデル</param>
			void RegistGeometry(Model& model)
			{
				//レイトレワールドにジオメトリを登録。
				m_world.RegistGeometry(model);
			}
			/// <summary>
			/// レイトレの結果の出力先となるテクスチャを取得。
			/// </summary>
			/// <returns></returns>
			Texture& GetOutputTexture()
			{
				return m_outputTexture;
			}
			/// <summary>
			/// ジオメトリの登録を確定。
			/// </summary>
			void CommitRegistGeometry(RenderContext& rc);
			/// <summary>
			/// シェーダーリソースを作成。
			/// </summary>
			void CreateShaderResources(const InitData& initData);
		private:
			/// <summary>
			/// シェーダーテーブルを作成。
			/// </summary>
			/// <param name="rc"></param>
			void CreateShaderTable(RenderContext& rc);

		private:
			/// <summary>
			/// カメラ
			/// </summary>
			struct Camera {
				Matrix mRot;	//回転行列
				Vector3 pos;	//視点。
				float aspect;	//アスペクト比。
				float fFar;		//遠平面。
				float fNear;	//近平面。
			};
			ExpanadSRVPtr m_expandSRV;					// 拡張シェーダーリソースビュー。
			ConstantBuffer m_rayGenerationCB;			//レイジェネレーションの定数バッファ。
			World m_world;								//レイトレワールド。
			PSO m_pipelineStateObject;					//パイプラインステートオブジェクト
			ShaderTable m_shaderTable;					//シェーダーテーブル。
			DescriptorHeaps m_descriptorHeaps;			//レイトレで使用するディスクリプタヒープの管理者。
			Texture m_outputTexture;					// レイトレースの結果の出力先(テクスチャ)
			GPUBuffer m_outputResource;					//レイトレースの結果の出力先。
		};
	}//namespace raytracing
}//namespace nsK2EngineLow 