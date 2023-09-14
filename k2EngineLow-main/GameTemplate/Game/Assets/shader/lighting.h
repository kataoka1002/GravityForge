//////ココから関数/////////////////////////////////////////////////////////////////////////////////////////////////////

//拡散反射の計算
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);

    float3 lig = color * power;

    return lig;
}

//鏡面反射の計算
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow)
{
    float3 toEye = normalize(eyePos - worldPos);

    float3 refVec = reflect(direction, normal);

    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);

    float3 lig = color * power * speclarPow;

    return lig;
}

//ディレクションライトの計算
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;

    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal, specularPow);
    }

    return lig;
}

//PBRのディレクションライトの計算
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        // PBRによるライトの強さを求める
        lig += CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, directionLight[i].dirDirection, directionLight[i].dirColor);
    }

    return lig;
}

//ポイントライトの計算
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;

    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
        // このサーフェイスに入射しているポイントライトの光の向きを計算する
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // 正規化して大きさ1のベクトルにする
        ligDir = normalize(ligDir);

        // 減衰なしのLambert拡散反射光を計算する
        float3 diffPoint = CalcLambertDiffuse(ligDir, pointLight[i].ptColor, normal);

        // 減衰なしのPhong鏡面反射光を計算する
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal, specularPow);

        // 距離による影響率を計算する
        // ポイントライトとの距離を計算する
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // 影響力がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
        affect = pow(affect, 3.0f);

        // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
        diffPoint *= affect;
        specPoint *= affect;

        // 最終的に返す強さに足していく
        lig += diffPoint + specPoint;
    }

    return lig;
}

//PBRのポイントライトを計算
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
        // このサーフェイスに入射しているポイントライトの光の向きを計算する
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // 正規化して大きさ1のベクトルにする
        ligDir = normalize(ligDir);

        // PBRによる光の強さを計算する        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, pointLight[i].ptColor);

        // 距離による影響率を計算する
        // ポイントライトとの距離を計算する
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // 影響力がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
        affect = pow(affect, 3.0f);

        // 減衰率を乗算して影響を弱める
        lig += PBRLig * affect;
    }

    return lig;
}

//スポットライトの計算
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //サーフェイスに入射するスポットライトの光の向きを計算する
        float3 LigDir = worldPos - spotLight[i].spPosition;
        //正規化
        LigDir = normalize(LigDir);

        //減衰なしのLambert拡散反射光を計算する
        float3 diffSpot = CalcLambertDiffuse(LigDir, spotLight[i].spColor, normal);

        //減衰なしのPhong鏡面反射の計算
        float3 specSpot = CalcPhongSpecular(LigDir, spotLight[i].spColor, worldPos, normal, specularPow);

        //スポットライトとの距離を計算する
        float distance = length(worldPos - spotLight[i].spPosition);

        //影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
        //影響力がマイナスにならないように
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        //影響を指数関数的にする
        affect = pow(affect, 3.0f);

        //拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
        diffSpot *= affect;
        specSpot *= affect;

        // ここからは角度による影響率を求める

        //入射光と射出方向の角度を求める
        float angle = dot(LigDir, spotLight[i].spDirection);

        //dot()で求めた値をacos()に渡して角度を求める
        angle = abs(acos(angle));

        //角度に比例して小さくなっていく影響率を計算する
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        //影響を指数関数的にする
        affect = pow(affect, 0.5f);

        //角度による影響率を反射光に乗算して、影響を弱める
        diffSpot *= affect;
        specSpot *= affect;

        //最終的に返す強さに足していく
        lig += diffSpot;
        lig += specSpot;

    }

    return lig;
}

//PBRのスポットライトを計算
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //サーフェイスに入射するスポットライトの光の向きを計算する
        float3 ligDir = worldPos - spotLight[i].spPosition;
        //正規化
        ligDir = normalize(ligDir);

        // PBRによる光の強さを計算する        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, spotLight[i].spColor);

        //スポットライトとの距離を計算する
        float distance = length(worldPos - spotLight[i].spPosition);

        //影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
        //影響力がマイナスにならないように
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        //影響を指数関数的にする
        affect = pow(affect, 3.0f);

        //減衰率を乗算して影響を弱める
        PBRLig *= affect;

        // ここからは角度による影響率を求める

        //入射光と射出方向の角度を求める
        float angle = dot(ligDir, spotLight[i].spDirection);

        //dot()で求めた値をacos()に渡して角度を求める
        angle = abs(acos(angle));

        //角度に比例して小さくなっていく影響率を計算する
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        //影響を指数関数的にする
        affect = pow(affect, 0.5f);

        //角度による影響率を反射光に乗算して、影響を弱める
        PBRLig *= affect;

        //最終的に返す強さに足していく
        lig += PBRLig;
    }

    return lig;
}

//リムライトの計算
float3 CalcLimPower(float3 normal, float3 normalInView)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        //サーフェイスの法線と光の入射方向に依存するリムの強さを求める
        float power1 = 1.0f - max(0.0f, dot(directionLight[i].dirDirection, normal));

        //サーフェイスの法線と視線の方向に依存するリムの強さを求める
        float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);

        //最終的なリムの強さを求める
        float limPower = power1 * power2;

        //pow()を使用し強さの変化を指数関数的にする
        limPower = pow(limPower, 1.3f);

        //リムライトのカラーを計算し足していく
        lig += directionLight[i].dirColor * limPower;
    }

    return lig;
}

//半球ライトの計算
float3 CalcHemLight(float3 normal)
{
    //サーフェイスの法線と地面の法線との内積を計算する
    float t = dot(normal, hemLight.groundNormal);

    //内積の結果を0～1の範囲に変換
    t = (t + 1.0f) / 2.0f;

    //地面と天球色を補完率tで線形補完し、返す
    return lerp(hemLight.groundColor, hemLight.skyColor, t);
}

//ベックマン分布を計算する
float CalcBeckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

//フレネルを計算
float CalcSpcFresnel(float f0, float u)
{
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算する
    float D = CalcBeckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = CalcSpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 粗さは0.5で固定。
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(L, H));

    // 光源に向かうベクトルとハーフベクトル、
    // 光が平行に入射したときの拡散反射量を求めている
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // 法線と光源に向かうベクトルwを利用して拡散反射率を求める
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (FL * FV * energyFactor);
}

float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color)
{
    // フレネル反射を考慮した拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -direction, toEye);

    // 正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -direction));
    float3 lambertDiffuse = color * NdotL / PI;

    // 最終的な拡散反射光を計算する
    float3 diffuse = diffuseFromFresnel * lambertDiffuse;

    // Cook-Torranceモデルを利用した鏡面反射率を計算する
    float3 spec = CookTorranceSpecular(-direction, toEye, normal, smooth) * color;

    // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
    // スペキュラカラーの強さを鏡面反射率として扱う
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    // 滑らかさが高ければ、拡散反射は弱くなる
    return diffuse * (1.0f - smooth) + spec / albedo;
}
