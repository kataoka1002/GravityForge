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
		/// ������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="animationClipsNum">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadowReciever">�V���h�E���V�[�o�[�t���O(true�Ȃ�e��������)</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClips = nullptr,
			int animationClipsNum = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = false,
			int maxInstance = 1);

		/// <summary>
		/// �C���X�^���V���O�`��p�̃��f���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="animationClipsNum">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadowReciever">�e���󂯂邩�ǂ���</param>
		/// <param name="maxInstance">�C���X�^���X�̍ő吔</param>
		void InitInstancing(
			const char* filePath,
			AnimationClip* animationClips = nullptr,
			int animationClipsNum = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = false,
			int maxInstance = 1);

		/// <summary>
		/// �C���X�^���V���O�`��p�̏��������������s�B
		/// </summary>
		/// <param name="maxInstance">�ő�C���X�^���X��</param>
		void InitInstancingDraw(int maxInstance);

		/// <summary>
		/// ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		/// </summary>
		/// <param name="modelInitData">���f���C�j�b�g�f�[�^</param>
		void SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData);

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �C���X�^���V���O�f�[�^�̍X�V�B
		/// </summary>
		/// <param name="instanceNo">�C���X�^���X�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale);

		/// <summary>
		/// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z�������������B
		/// </summary>
		/// <param name="tkmFilePath">tkm�t�@�C���̃t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏㎲</param>
		void InitComputeAnimatoinVertexBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �C���X�^���X�������B
		/// </summary>
		/// <remark>
		/// �C���X�^���X�`��𗘗p���Ă���ۂɁA���̃C���X�^���X���V�[�����珜���������ꍇ�ɗ��p���Ă��������B
		/// </remark>
		/// <param name="instanceNo">�C���X�^���X�ԍ�</param>
		void RemoveInstance(int instanceNo);

		void ResetInstance(int instanceNo);

		/// <summary>
		/// ���_�v�Z�p�X����Ăяo����鏈���B
		/// </summary>
		/// <param name="rc"></param>
		void OnComputeVertex(RenderContext& rc);

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// ���W�ݒ�
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
		/// ��]�ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// �T�C�Y�ݒ�
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
		/// ���W�g���]��ݒ�
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
		/// ���f���̍��W���擾
		/// </summary>
		/// <returns></returns>
		Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// ���f����X���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionX()
		{
			return m_position.x;
		}

		/// <summary>
		/// ���f����Y���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionY()
		{
			return m_position.y;
		}

		/// <summary>
		/// ���f����Z���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionZ()
		{
			return m_position.z;
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
		/// <param name="interpolateTime">�⊮����</param>
		void PlayAnimation(int animNum, float interpolateTime = 0.0f)
		{
			m_nowAnimationNumber = animNum;
			m_animation.Play(animNum, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// �A�j���[�V�����i���o�[�̎擾
		/// </summary>
		/// <returns></returns>
		int GetAnimationNum() const
		{
			return m_nowAnimationNumber;
		}

		/// <summary>
		/// �f�B�t�@�[�h�p�̃��f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void OnDraw(RenderContext& rc)
		{
			m_renderToGBufferModel.Draw(rc, 1);
		}

		/// <summary>
		/// �V���h�E���f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void OnShadowModelDraw(RenderContext& rc)
		{
			//m_shadowDrawModel.Draw(rc, g_renderingEngine->GetLightCamera());
		}

		void UpdateShadowCameraParam(int shadowMapNo)
		{
			Vector4 cameraParam;
			cameraParam.x = g_camera3D->GetNear();
			cameraParam.y = g_camera3D->GetFar();
			m_drawShadowMapCameraParamCB[shadowMapNo].CopyToVRAM(cameraParam);
		}

		/// <summary>
		/// ZPrepass���f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc)
		{
			m_zprepassModel.Draw(rc, 1);
		}

		/// <summary>
		/// �X�J�C�L���[�u�̏�����
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData)
		{
			m_renderToGBufferModel.Init(initData);
		}

		/// <summary>
		/// ���f�����擾�B
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			if (m_renderToGBufferModel.IsInited()) 
			{
				return m_renderToGBufferModel;
			}
		}

		/// <summary>
		/// �A�j���[�V�����C�x���g�̒ǉ�
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �C���X�^���V���O�`����s���H
		/// </summary>
		/// <returns></returns>
		bool IsInstancingDraw() const
		{
			return m_isEnableInstancingDraw;
		}

		/// <summary>
		/// ���[���h�s����擾�B
		/// </summary>
		/// <param name="instanceId">
		/// �C���X�^���XID�B
		/// ���̈����̓C���X�^���V���O�`�悪�������̏ꍇ�͖�������܂��B
		/// </param>
		/// <returns></returns>
		const Matrix& GetWorldMatrix(int instanceId) const
		{
			if (IsInstancingDraw()) {
				return m_worldMatrixArray[instanceId];
			}
			return m_zprepassModel.GetWorldMatrix();
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�̃t���O��ݒ肷��
		/// </summary>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�H
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster() const
		{
			return m_isShadowCaster;
		}

		/// <summary>
		/// �J�����O�̑Ώۂ���O��
		/// </summary>
		void SetDontCulling()
		{
			m_doCulling = false;
		}

		void SetAnimationSpeed(float speed)
		{
			m_animationSpeed = speed;
		}

	private:

		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// GBuffer�`�惂�f���̏�����
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever);

		/// <summary>
		/// �V���h�E�p�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// ZPrepass�`��p�̃��f�����������B
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModelOnZprepass(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		/// <summary>
		/// �C���X�^���V���O�`��p�̃��f���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitInstancingModelOnRenderGBuffer(
			const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ);

		/// <summary>
		/// �C���X�^���V���O�`��p��ZPpepass���f���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitInstancingModelOnZprepass(
			const char* filePath,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �C���X�^���V���O�`��p�̃V���h�E���f��
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitInstancingShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// ���C�g�����[���h�ɓo�^���邩�̃t���O��ݒ肷��
		/// �p�ɂɐ������郂�f���i�e�Ȃǁj��Init����O�Ɉ���false�Ŏ��s���Ă��������B
		/// </summary>
		void SetRaytracingWorld(bool flag) 
		{
			m_isRaytracingWorld = flag;
		}

		/// <summary>
		/// �􉽊w����������
		/// </summary>
		/// <param name="maxInstance">�C���X�^���X��</param>
		void InitGeometryDatas(int maxInstance);

	private:
		Model m_zprepassModel;								// ZPrepass�ŕ`�悳��郂�f��
		Model m_forwardRenderModel;							// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		Model m_renderToGBufferModel;						// RenderToGBuffer�ŕ`�悳��郂�f��
		Model m_shadowDrawModel[NUM_SHADOW_MAP];			// �V���h�E�}�b�v�ɕ`�悷�郂�f��
		Model* m_addRaytracingWorldModel = nullptr;			// ���C�g�����[���h�ɓo�^�������f���B				
		Vector3 m_position = Vector3::Zero;					// �|�W�V����
		Vector3 m_scale = Vector3::One;						// �X�P�[��
		Quaternion m_rotation = { 0.0f,0.0f,0.0f,1.0f };	// ��]
		Animation m_animation;								// �A�j���[�V����
		AnimationClip* m_animationClips;					// �A�j���[�V�����N���b�v
		Skeleton m_skeleton;								// �X�P���g��
		int m_numAnimationClips = 0;						// �A�j���[�V�����N���b�v�i���o�[
		int m_nowAnimationNumber = -1;						// ���̃A�j���[�V�����N���b�v�i���o�[
		int	m_numInstance = 0;								// �C���X�^���X�̐��B
		int	m_maxInstance = 1;								// �ő�C���X�^���X���B
		float m_animationSpeed = 1.0f;						// �A�j���[�V�����̑���
		bool m_isRaytracingWorld = true;					// ���C�g�����[���h�ɓo�^����H
		bool m_isShadowCaster = true;						// �V���h�E�L���X�^�[�t���O
		bool m_doCulling = true;							// �J�����O�����邩�ǂ���
		bool m_isEnableInstancingDraw = false;				// �C���X�^���V���O�`�悪�L���H

		std::vector< GemometryData > m_geometryDatas;		// �W�I���g�����B
		std::unique_ptr<Matrix[]>	 m_worldMatrixArray;	// ���[���h�s��̔z��B
		StructuredBuffer			 m_worldMatrixArraySB;	// ���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
		std::unique_ptr<int[]> m_instanceNoToWorldMatrixArrayIndexTable;	// �C���X�^���X�ԍ����烏�[���h�s��̔z��̃C���f�b�N�X�ɕϊ�����e�[�u���B
		ConstantBuffer m_drawShadowMapCameraParamCB[NUM_SHADOW_MAP];		// �V���h�E�}�b�v�쐬���ɕK�v�ȃJ�����p�����[�^�p�̒萔�o�b�t�@�B
		ComputeAnimationVertexBuffer m_computeAnimationVertexBuffer;		// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z�����B
	};
}


