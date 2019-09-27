//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;
float gRp;
float gGp;
float gBp;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepthWriting
{
    DepthEnable = true;
    DepthWriteMask = ALL;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
    CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    float4 color = gTexture.Sample(samPoint, input.TexCoord);

    color.r = color.r + mul(color.r, gRp);
    color.g = color.g + mul(color.g, gGp);
    color.b = color.b + mul(color.b, gBp);

    clamp(color.r, 0, 1);
    clamp(color.g, 0, 1);
    clamp(color.b, 0, 1);
    return color;
}


//TECHNIQUE
//---------
technique11 Grayscale
{
    pass P0
    {          
		// Set states...
        SetRasterizerState(BackCulling);
        SetDepthStencilState(EnableDepthWriting, 0);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}

