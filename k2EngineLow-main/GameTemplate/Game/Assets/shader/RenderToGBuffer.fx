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
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
    float3 tangent : TANGENT;
    float3 biNomal : BINORMAL;
};

//�s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; //���W�B
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2;
    float3 tangent : TANGENT;
    float3 biNomal : BINORMAL;
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float4 albedo : SV_Target0;                 // �A���x�h
    float4 normal : SV_Target1;                 // �@��
    float3 worldPos : SV_Target2;               // ���[���h���W
    float3 normalInView : SV_Target3;           // �J������Ԃ̖@��
    float4 metallicSmooth : SV_Target4;         // ���^���b�N�X���[�X
};


//�V�F�[�_�[���\�[�X
Texture2D<float4> g_albedo : register(t0);              // �A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1);           // �@���}�b�v
Texture2D<float4> g_metallicSmoothMap : register(t2);   // ���^���b�N�X���[�X(�X�y�L����)�}�b�v�Br�Ƀ��^���b�N�Aa�ɃX���[�X
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // �{�[���s��
StructuredBuffer<float4x4> g_worldMatrixArray : register(t10); //���[���h�s��̔z��B�C���X�^���V���O�`��̍ۂɗL���B

//�T���v���[�X�e�[�g
sampler g_sampler : register(s0);

float3 CalcNormal(SPSIn psIn);

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

//���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
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

    psIn.pos = mul(m, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.normalInView = mul(mView, psIn.normal); // �J������Ԃ̖@�������߂�
    psIn.tangent = normalize(mul(m, vsIn.tangent)); // �ڃx�N�g�������[���h��Ԃɕϊ�����
    psIn.biNomal = normalize(mul(m, vsIn.biNomal)); // �]�x�N�g�������[���h��Ԃɕϊ�����

    psIn.uv = vsIn.uv;
    
    return psIn;
}

// �C���X�^���V���O���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCoreInstancing(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    SPSIn psIn;
    float4x4 m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.normalInView = mul(mView, psIn.normal);    // �J������Ԃ̖@�������߂�
    psIn.tangent = normalize(mul(m, vsIn.tangent)); // �ڃx�N�g�������[���h��Ԃɕϊ�����
    psIn.biNomal = normalize(mul(m, vsIn.biNomal)); // �]�x�N�g�������[���h��Ԃɕϊ�����

    psIn.uv = vsIn.uv;
    
    return psIn;
}

/// ���O�v�Z�ς݂̒��_�o�b�t�@���g�����_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMainSkinUsePreComputedVertexBuffer(SVSIn vsIn)
{
    SPSIn psIn;
    
    psIn.pos = vsIn.pos;
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = vsIn.normal;
    psIn.normalInView = mul(mView, psIn.normal); // �J������Ԃ̖@�������߂�
    psIn.tangent = vsIn.tangent; // �ڃx�N�g�������[���h��Ԃɕϊ�����
    psIn.biNomal = vsIn.biNomal; // �]�x�N�g�������[���h��Ԃɕϊ�����
    psIn.uv = vsIn.uv;
    
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


//���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
SPSOut PSMainCore(SPSIn psIn, uniform bool isShadowReceiver)
{
    //GBuffer�ɏo��
    SPSOut psOut;
    
    //�A���x�h�J���[�̒��o
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�@���}�b�v�ɂ��@�����̒��o
    psOut.normal.xyz = CalcNormal(psIn);
    
    //�e�𗎂Ƃ���邾�����𒊏o
    if (isShadowReceiver == true)
    {
        psOut.normal.w = 1.0f;
    }
    else
    {
        psOut.normal.w = 0.0f;
    }
    
    //���[���h���W�̒��o
    psOut.worldPos = psIn.worldPos;
    
    //�J������Ԃ̖@���̒��o
    psOut.normalInView = psIn.normalInView;
    
    //���^���b�N�X���[�X�}�b�v(�X�y�L�����}�b�v)�̒��o
    psOut.metallicSmooth = g_metallicSmoothMap.Sample(g_sampler, psIn.uv);
    
    
    return psOut;
}

//�e�𗎂Ƃ��Ȃ����󂯂Ȃ�
SPSOut PSMain(SPSIn psIn) : SV_Target0
{
    return PSMainCore(psIn, false);
}

//�e�𗎂Ƃ������Ƃ����ق�            �V���h�E���V�[�o�[(�e�𗎂Ƃ����󂯂邾���̕�)�̃G���g���[�|�C���g
SPSOut PSShadowMain(SPSIn psIn) : SV_Target0
{
    return PSMainCore(psIn, true);
}


////�R�R����֐�/////////////////////////////////////////////////////////////////////

float3 CalcNormal(SPSIn psIn)
{
    // �@���}�b�v����^���W�F���g�X�y�[�X�̖@�����T���v�����O����
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    
    // �^���W�F���g�X�y�[�X�̖@�������[���h�X�y�[�X�ɕϊ�����
    float3 normal = psIn.tangent * localNormal.x + psIn.biNomal * localNormal.y + psIn.normal * localNormal.z;
    
    // �o�͂�0�`1�Ɋۂ߂��Ă��܂��}�C�i�X�̒l�������Ă��܂��̂�-1�`1��0�`1�ɕϊ�����
    normal = (normal / 2.0f) + 0.5f;

    return normal;
}