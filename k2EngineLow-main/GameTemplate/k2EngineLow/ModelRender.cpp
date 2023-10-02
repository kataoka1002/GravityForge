#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,	
		bool isShadowReciever)
	{
		// スケルトンを初期化。
		InitSkeleton(filePath);

		// アニメーションを初期化。
		InitAnimation(animationClips, animationClipsNum, enModelUpAxis);
		
		// GBuffer描画用のモデルを初期化
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);
		 
		// ZPrepass描画用のモデルを初期化
		 
		// 影を受ける側じゃないなら
		if (isShadowReciever != true)
		{
			// シャドウマップ描画用のモデルを初期化
			InitShadowDrawModel(filePath, enModelUpAxis);
		}

		// レイトレワールドに追加
		g_renderingEngine->AddModelToRaytracingWorld(m_renderToGBufferModel);
		m_addRaytracingWorldModel = &m_renderToGBufferModel;
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
		if (m_animationClips != nullptr) 
		{
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}

		if (isShadowReciever) 
		{
			modelInitData.m_psEntryPointFunc = "PSShadowMain";
		}
		else
		{
			modelInitData.m_psEntryPointFunc = "PSMain";
		}
		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::InitShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis)
	{
		//シャドウマップに書きこむモデルの設定
		ModelInitData sadowDrawModelInitData;
		sadowDrawModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
		sadowDrawModelInitData.m_psEntryPointFunc = "PSMain";
		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する。
			sadowDrawModelInitData.m_skeleton = &m_skeleton;
			sadowDrawModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			sadowDrawModelInitData.m_vsEntryPointFunc = "VSMain";
		}
		sadowDrawModelInitData.m_tkmFilePath = tkmFilePath;
		sadowDrawModelInitData.m_modelUpAxis = enModelUpAxis;
		sadowDrawModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		m_shadowDrawModel[0].Init(sadowDrawModelInitData);
		m_shadowDrawModel[1].Init(sadowDrawModelInitData);
		m_shadowDrawModel[2].Init(sadowDrawModelInitData);
	}

	void ModelRender::Update()
	{
		//モデル側に移動回転拡大を渡す
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[0].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[1].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[2].UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited()) 
		{
			//スケルトンの更新
			m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
		}

		//アニメーションを進める。
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddModelRenderObject(this);	
		g_renderingEngine->Add3DModelToRenderToShadowMap(m_shadowDrawModel[0], m_shadowDrawModel[1], m_shadowDrawModel[2]);
	}
}