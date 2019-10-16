#include "Share.fx"

cbuffer Button : register(b12)
{
    float4 g_vButtonColor;
}

cbuffer Image : register(b13)
{
    float4 g_vImageColor;
}

cbuffer Bar : register(b11)
{
    float4 vColor;
    float fRatio;
	int  iHeight;				// 세로여부
    float2 vEmpty;
}

// 버튼
VS_OUTPUT_UV VS_Button(VS_INPUT_UV _Input)
{
    VS_OUTPUT_UV    output = (VS_OUTPUT_UV) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vUV = _Input.vUV;
    
    return output;
}

PS_OUTPUT PS_Button(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT       output = (PS_OUTPUT)0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV) * g_vButtonColor;

    return output;

}

// 이미지
VS_OUTPUT_UV VS_Image(VS_INPUT_UV _Input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vUV = _Input.vUV;
    
    return output;
}

PS_OUTPUT PS_Image(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV) * g_vImageColor;

    return output;

}

// 바 ( 비율따짐 )
VS_OUTPUT_UV VS_Bar(VS_INPUT_UV _Input)
{
    VS_OUTPUT_UV    output = (VS_OUTPUT_UV) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vUV = _Input.vUV;
    
    return output;
}

PS_OUTPUT PS_Bar(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT       output = (PS_OUTPUT)0;

	if (iHeight >= 1)
	{
		// 세로일 경우
		if (_Input.vUV.y > fRatio)
			clip(-1);
	}
	else
	{
		if (_Input.vUV.x > fRatio)
			clip(-1);
	}

    if (fRatio == 0.0f)
        clip(-1);

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    return output;

}

// 바 - 월드용

//
PS_OUTPUT PS_ONLY_IMAGE(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

	if (fRatio < _Input.vUV.x)
	{
		clip(-1);
	}
    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    return output;

}

PS_OUTPUT PS_CoolTime(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    return output;

}


// 인스턴싱 UI
VS_OUTPUT_UV_INSTANCING VS_Button_Instancing(VS_INPUT_UV_INSTANCINGE _Input)
{
    VS_OUTPUT_UV_INSTANCING output = (VS_OUTPUT_UV_INSTANCING) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), _Input.matWVP);
    output.vUV = _Input.vUV;

    output.matWVP = _Input.matWVP;
    output.matWV = _Input.matWV;
    output.matWVRot = _Input.matWVRot;
    
    return output;
}

PS_OUTPUT PS_Button_Instancing(VS_OUTPUT_UV_INSTANCING _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV) * g_vButtonColor;

    return output;

}

// 이미지
VS_OUTPUT_UV_INSTANCING VS_Image_Instancing(VS_INPUT_UV_INSTANCINGE _Input)
{
    VS_OUTPUT_UV_INSTANCING output = (VS_OUTPUT_UV_INSTANCING) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), _Input.matWVP);
    output.vUV = _Input.vUV;

    output.matWVP = _Input.matWVP;
    output.matWV = _Input.matWV;
    output.matWVRot = _Input.matWVRot;
    
    return output;
}

PS_OUTPUT PS_Image_Instancing(VS_OUTPUT_UV_INSTANCING _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV) * g_vImageColor;

    return output;

}

// 바 ( 비율따짐 )
VS_OUTPUT_UV_INSTANCING VS_Bar_Instancing(VS_INPUT_UV_INSTANCINGE _Input)
{
    VS_OUTPUT_UV_INSTANCING output = (VS_OUTPUT_UV_INSTANCING) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), _Input.matWVP);
    output.vUV = _Input.vUV;

    output.matWVP = _Input.matWVP;
    output.matWV = _Input.matWV;
    output.matWVRot = _Input.matWVRot;
    
    return output;
}

PS_OUTPUT PS_Bar_Instancing(VS_OUTPUT_UV_INSTANCING _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    if (_Input.vUV.x > fRatio)
        clip(-1);

    if (fRatio == 0.0f)
        clip(-1);

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    return output;

}
