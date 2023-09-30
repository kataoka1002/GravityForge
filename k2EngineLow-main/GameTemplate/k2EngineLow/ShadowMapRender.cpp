#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	namespace shadow
	{
		void ShadowMapRender::Init()
		{
            float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            //�ߌi�p�̃V���h�E�}�b�v
            m_shadowMaps[0].Create(
                2048,
                2048,
                1,
                1,
                DXGI_FORMAT_R32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );

            //���i�p�̃V���h�E�}�b�v
            m_shadowMaps[1].Create(
                1024,
                1024,
                1,
                1,
                DXGI_FORMAT_R32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );

            //���i�p�̃V���h�E�}�b�v
            m_shadowMaps[2].Create(
                512,
                512,
                1,
                1,
                DXGI_FORMAT_R32_FLOAT,
                DXGI_FORMAT_D32_FLOAT,
                clearColor
            );
		}

		void ShadowMapRender::Render(RenderContext& rc, Vector3& lightDirection)
		{
            // ���C�g�r���[�v���W�F�N�V�����N���b�v�s����v�Z
            m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(lightDirection);

            for (int i = 0; i < NUM_SHADOW_MAP; i++)
            {
                // �s���萔�o�b�t�@�ɃZ�b�g����
                g_renderingEngine->SetLVP(GetLVPMatrix(i), i);
            }

            int shadowMapNo = 0;
            for (auto& shadowMap : m_shadowMaps)
            {
                // �^�[�Q�b�g���V���h�E�}�b�v�ɂ��ď�������
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

                // �`�悪�I�������N���A
                m_modelsArray[shadowMapNo].clear();

                // �������݊����҂�
                rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
                shadowMapNo++;
            }
		}
	}
}