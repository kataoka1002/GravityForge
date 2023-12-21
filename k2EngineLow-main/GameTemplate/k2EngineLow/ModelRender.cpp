#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{
		if (g_renderingEngine != nullptr) {
			//g_renderingEngine->RemoveEventListener(this);
			for (auto& geomData : m_geometryDatas) {
				// レンダリングエンジンから登録解除
				g_renderingEngine->UnregisterGeometryData(&geomData);
			}
			/*if (m_addRaytracingWorldModel) {
				g_renderingEngine->RemoveModelFromRaytracingWorld(*m_addRaytracingWorldModel);
			}*/
		}
	}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance)
	{
		// スケルトンを初期化。
		InitSkeleton(filePath);

		// アニメーションを初期化。
		InitAnimation(animationClips, animationClipsNum, enModelUpAxis);

		// GBuffer描画用のモデルを初期化
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		// シャドウマップ描画用のモデルを初期化
		InitShadowDrawModel(filePath, enModelUpAxis);

		// 幾何学データを初期化。
		InitGeometryDatas(maxInstance);

		// ZPrepass描画用のモデルを初期化
		InitModelOnZprepass(filePath, enModelUpAxis);

		// レイトレワールドに追加
		if (m_isRaytracingWorld)
		{
			g_renderingEngine->AddModelToRaytracingWorld(m_renderToGBufferModel);
			m_addRaytracingWorldModel = &m_renderToGBufferModel;
		}
	}

	void ModelRender::InitInstancing(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance)
	{
		// インスタンシング描画用のデータを初期化。
		InitInstancingDraw(maxInstance);

		// スケルトンを初期化。
		InitSkeleton(filePath);

		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";

		// 頂点シェーダーのエントリーポイントをセットアップ。
		modelInitData.m_vsEntryPointFunc = "VSMainCoreInstancing";
		// ピクセルシェーダーのエントリーポイントをセットアップ。
		modelInitData.m_psEntryPointFunc = "PSShadowMain";
		
		//モデルの上方向を指定する。
		modelInitData.m_modelUpAxis = enModelUpAxis;

		modelInitData.m_tkmFilePath = filePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
		modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		m_renderToGBufferModel.Init(modelInitData);


		// シャドウマップ描画用のモデルを初期化
		//InitShadowDrawModel(filePath, enModelUpAxis);

		// 幾何学データを初期化。
		InitGeometryDatas(maxInstance);

		// ZPrepass描画用のモデルを初期化
		//InitModelOnZprepass(filePath, enModelUpAxis);
		ModelInitData modelInitDataZ;
		modelInitDataZ.m_tkmFilePath = filePath;
		modelInitDataZ.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitDataZ.m_modelUpAxis = enModelUpAxis;

		// 頂点シェーダーのエントリーポイントをセットアップ。
		modelInitDataZ.m_vsEntryPointFunc = "VSMainCoreInstancing";

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitDataZ.m_skeleton = &m_skeleton;
		}

		modelInitDataZ.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		if (m_isEnableInstancingDraw) {
			// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			modelInitDataZ.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		}

		m_zprepassModel.Init(modelInitDataZ);
	}

	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;
		if (m_maxInstance > 1) {
			// インスタンシング描画を行うので、それ用のデータを構築する。
			// ワールド行列の配列のメモリを確保する。
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			// ワールド行列をGPUに転送するためのストラクチャードバッファを確保。
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);
			m_isEnableInstancingDraw = true;
			// インスタンス番号からワールド行列の配列のインデックスに変換するテーブルを初期化する。
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
		}
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		//アニメーションの設定
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, numAnimationClips);
		}
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//アニメーションがあるなら
		if (m_animationClips != nullptr)
		{
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
	}

	void ModelRender::InitGeometryDatas(int maxInstance)
	{
		m_geometryDatas.resize(maxInstance);
		int instanceId = 0;
		for (auto& geomData : m_geometryDatas) {
			geomData.Init(this, instanceId);
			// レンダリングエンジンに登録。
			g_renderingEngine->RegisterGeometryData(&geomData);
			instanceId++;
		}
	}

	void ModelRender::InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";
		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[4] = DXGI_FORMAT_R8G8B8A8_UNORM;

		//スケルトンを指定する。
		if (m_animationClips != nullptr)
		{
			modelInitData.m_skeleton = &m_skeleton;
		}

		//頂点シェーダーのエントリーポイントを設定
		SetupVertexShaderEntryPointFunc(modelInitData);

		//ピクセルシェーダーのエントリーポイントを設定
		//if (isShadowReciever) 
		{
			modelInitData.m_psEntryPointFunc = "PSShadowMain";
		}
		/*else
		{
			modelInitData.m_psEntryPointFunc = "PSMain";
		}*/

		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::InitShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis)
	{
		//シャドウマップに書きこむモデルの設定
		ModelInitData sadowDrawModelInitData;
		sadowDrawModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
		sadowDrawModelInitData.m_psEntryPointFunc = "PSMain";
		sadowDrawModelInitData.m_tkmFilePath = tkmFilePath;
		sadowDrawModelInitData.m_modelUpAxis = enModelUpAxis;
		sadowDrawModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

		//スケルトンを指定する。
		if (m_animationClips != nullptr)
		{
			sadowDrawModelInitData.m_skeleton = &m_skeleton;
		}

		//頂点シェーダーのエントリーポイントを設定
		SetupVertexShaderEntryPointFunc(sadowDrawModelInitData);

		m_shadowDrawModel[0].Init(sadowDrawModelInitData);
		m_shadowDrawModel[1].Init(sadowDrawModelInitData);
		m_shadowDrawModel[2].Init(sadowDrawModelInitData);
	}

	void ModelRender::InitModelOnZprepass(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		// 頂点シェーダーのエントリーポイントをセットアップ。
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//if (m_isEnableInstancingDraw) {
		//	// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
		//	modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		//}

		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}

		//モデル側に移動回転拡大を渡す
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[0].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[1].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[2].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited())
		{
			//スケルトンの更新
			m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
		}

		//アニメーションを進める。
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::RemoveInstance(int instanceNo)
	{
		int matrixIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];

		m_worldMatrixArray[matrixIndex] = g_matZero;
	}

	void ModelRender::UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(instanceNo < m_maxInstance, "インスタンス番号が不正です。");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		//if (m_translucentModel.IsInited()) {
		//	// 半透明モデルはZPrepassモデルを初期化していないので、こちらを使う。
		//	worldMatrix = m_translucentModel.CalcWorldMatrix(pos, rot, scale);
		//}
		/*else */{
			worldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);
		}
		// インスタンス番号から行列のインデックスを取得する。
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// インスタンシング描画を行う。
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める。
			//m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	int g_hoge = 0;
	void ModelRender::Draw(RenderContext& rc)
	{
		//カリングの対象じゃないなら常時表示
		if (m_doCulling == false)
		{
			g_renderingEngine->AddModelRenderObject(this);
			return;
		}

		if (m_isEnableInstancingDraw) {
			// インスタンシング描画はビューフラスタムカリングは行わない。
			g_renderingEngine->AddModelRenderObject(this);
			m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
			m_numInstance = 0;
		}
		else {
			// 通常描画
			if (m_geometryDatas[0].IsInViewFrustum()) {
				g_renderingEngine->AddModelRenderObject(this);
				g_renderingEngine->Add3DModelToRenderToShadowMap(m_shadowDrawModel[0], m_shadowDrawModel[1], m_shadowDrawModel[2]);
			}
		}

		//// ビューフラスタムに含まれているなら
		//if (m_geometryDatas[0].IsInViewFrustum()) 
		//{
		//g_hoge++;
		//}
	}
}