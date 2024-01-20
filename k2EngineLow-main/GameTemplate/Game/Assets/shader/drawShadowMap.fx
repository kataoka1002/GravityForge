/*!
 * @brief �V���h�E�}�b�v�`��p�̃V�F�[�_�[
 */

static const int INFINITY = 40.0f;

// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0; // UV���W
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0; // uv���W
};

///////////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0); // �A���x�h�}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // �{�[���s��
//Texture2D<float4> g_shadowMap : register(t10); // �V���h�E�}�b�v
sampler g_sampler : register(s0); // �T���v���[�X�e�[�g
StructuredBuffer<float4x4> g_worldMatrixArray : register(t10); //���[���h�s��̔z��B�C���X�^���V���O�`��̍ۂɗL���B


//�X�L���s����v�Z����B
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// ���_�V�F�[�_�[
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, bool hasSkin)
{
    // �V���h�E�}�b�v�`��p�̒��_�V�F�[�_�[������
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(m, vsIn.normal);
    return psIn;
}

// �C���X�^���V���O���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCoreInstancing(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    SPSIn psIn;
    
    float4x4 m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(m, vsIn.normal);
    
    return psIn;
}

//SPSIn VSMainCoreInstancingAnim(SVSIn vsIn, uint instanceId : SV_InstanceID)
//{
//    SPSIn psIn;
//    float4x4 mWorldLocal = CalcSkinMatrix(vsIn.skinVert);
//    mWorldLocal = mul(g_worldMatrixArray[instanceId], mWorldLocal);
//    psIn.pos = mul(mWorldLocal, vsIn.pos);
//    psIn.pos = mul(mView, psIn.pos);
//    psIn.pos = mul(mProj, psIn.pos);
//    psIn.uv = vsIn.uv;
//    psIn.normal = mul(mWorldLocal, vsIn.normal);
    
//    return psIn;
//}

/// ���O�v�Z�ς݂̒��_�o�b�t�@���g�����_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMainSkinUsePreComputedVertexBuffer(SVSIn vsIn)
{
    SPSIn psIn;
    
    psIn.pos = vsIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = vsIn.normal;
    
    return psIn;
}

// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}


/// <summary>
/// �V���h�E�}�b�v�`��p�̃s�N�Z���V�F�[�_�[
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // �V���h�E�}�b�v��Z�l��`������
    //return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
    
    float depth = psIn.pos.z;
    float pos = exp(INFINITY * depth);
    return float4(pos, pos * pos, 0.0f, 1.0f);
}