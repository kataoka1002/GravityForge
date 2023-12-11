#pragma once

#include "geometry/AABB.h"
#include "GeometryData.h"
#include "graphics/ComputeAnimationVertexBuffer.h"

namespace nsK2EngineLow
{
	class ModelRender : public Noncopyable
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="animationClipsNum">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowReciever">シャドウレシーバーフラグ(trueなら影が落ちる)</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClips = nullptr,
			int animationClipsNum = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = false,
			int maxInstance = 1);

		/// <summary>
		/// 頂点シェーダーのエントリーポイントを指定する
		/// </summary>
		/// <param name="modelInitData">モデルイニットデータ</param>
		void SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// 座標設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		void SetPosition(const float x, const float y, const float z)
		{
			SetPosition({ x,y,z });
		}

		/// <summary>
		/// 回転設定
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// サイズ設定
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(const float x, const float y, const float z)
		{
			SetScale({ x,y,z });
		}
		void SetScale(const float xyz)
		{
			SetScale({ xyz ,xyz ,xyz });
		}

		/// <summary>
		/// 座標拡大回転を設定
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rotation"></param>
		/// <param name="scale"></param>
		void SetTRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// モデルの座標を取得
		/// </summary>
		/// <returns></returns>
		Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// モデルのX座標を取得
		/// </summary>
		/// <returns></returns>
		float& GetPositionX()
		{
			return m_position.x;
		}

		/// <summary>
		/// モデルのY座標を取得
		/// </summary>
		/// <returns></returns>
		float& GetPositionY()
		{
			return m_position.y;
		}

		/// <summary>
		/// モデルのZ座標を取得
		/// </summary>
		/// <returns></returns>
		float& GetPositionZ()
		{
			return m_position.z;
		}

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号</param>
		/// <param name="interpolateTime">補完時間</param>
		void PlayAnimation(int animNum, float interpolateTime = 0.0f)
		{
			m_nowAnimationNumber = animNum;
			m_animation.Play(animNum, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// アニメーションナンバーの取得
		/// </summary>
		/// <returns></returns>
		int GetAnimationNum() const
		{
			return m_nowAnimationNumber;
		}

		/// <summary>
		/// ディファード用のモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void OnDraw(RenderContext& rc)
		{
			m_renderToGBufferModel.Draw(rc);
		}

		/// <summary>
		/// シャドウモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void OnShadowModelDraw(RenderContext& rc)
		{
			//m_shadowDrawModel.Draw(rc, g_renderingEngine->GetLightCamera());
		}

		/// <summary>
		/// ZPrepassモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc)
		{
			m_zprepassModel.Draw(rc);
		}

		/// <summary>
		/// スカイキューブの初期化
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData)
		{
			m_renderToGBufferModel.Init(initData);
		}

		/// <summary>
		/// モデルを取得。
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			if (m_renderToGBufferModel.IsInited()) 
			{
				return m_renderToGBufferModel;
			}
		}

		/// <summary>
		/// アニメーションイベントの追加
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <param name="instanceId">
		/// インスタンスID。
		/// この引数はインスタンシング描画が向こうの場合は無視されます。
		/// </param>
		/// <returns></returns>
		const Matrix& GetWorldMatrix(int instanceId) const
		{
			/*if (IsInstancingDraw()) {
				return m_worldMatrixArray[instanceId];
			}*/
			return m_zprepassModel.GetWorldMatrix();
		}

		/// <summary>
		/// シャドウキャスターのフラグを設定する
		/// </summary>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// シャドウキャスター？
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster() const
		{
			return m_isShadowCaster;
		}

		/// <summary>
		/// カリングの対象から外す
		/// </summary>
		void SetDontCulling()
		{
			m_doCulling = false;
		}

	private:

		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上向き</param>
		void InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// GBuffer描画モデルの初期化
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever);

		/// <summary>
		/// シャドウ用のモデルの初期化
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// ZPrepass描画用のモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModelOnZprepass(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		/// <summary>
		/// レイトレワールドに登録するかのフラグを設定する
		/// 頻繁に生成するモデル（弾など）をInitする前に引数falseで実行してください。
		/// </summary>
		void SetRaytracingWorld(bool flag) 
		{
			m_isRaytracingWorld = flag;
		}

		/// <summary>
		/// 幾何学情報を初期化
		/// </summary>
		/// <param name="maxInstance">インスタンス数</param>
		void InitGeometryDatas(int maxInstance);

	private:
		Model m_zprepassModel;								// ZPrepassで描画されるモデル
		Model m_forwardRenderModel;							// フォワードレンダリングの描画パスで描画されるモデル
		Model m_renderToGBufferModel;						// RenderToGBufferで描画されるモデル
		Model m_shadowDrawModel[NUM_SHADOW_MAP];			// シャドウマップに描画するモデル
		Model* m_addRaytracingWorldModel = nullptr;			// レイトレワールドに登録したモデル。				
		Vector3 m_position = Vector3::Zero;					// ポジション
		Vector3 m_scale = Vector3::One;						// スケール
		Quaternion m_rotation = { 0.0f,0.0f,0.0f,1.0f };	// 回転
		Animation m_animation;								// アニメーション
		AnimationClip* m_animationClips;					// アニメーションクリップ
		Skeleton m_skeleton;								// スケルトン
		int m_numAnimationClips = 0;						// アニメーションクリップナンバー
		int m_nowAnimationNumber = -1;						// 今のアニメーションクリップナンバー
		float m_animationSpeed = 1.0f;						// アニメーションの速さ
		bool m_isRaytracingWorld = true;					// レイトレワールドに登録する？
		bool m_isShadowCaster = true;						// シャドウキャスターフラグ
		bool m_doCulling = true;							// カリングをするかどうか

		std::vector< GemometryData > m_geometryDatas;		// ジオメトリ情報。
	};
}


