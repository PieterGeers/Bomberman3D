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

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepthWriting
{
    DepthEnable = true;
    DepthWriteMask = ALL;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
    CullMode = NONE;
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
	// Step 1: find the dimensions of the texture (the texture has a method for that)
    uint width = 0;
    uint height = 0;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
    float dx = 1.0f / width;
    float dy = 1.0f / height;
	// Step 3: Create a double for loop (5 iterations each)
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float totalPass = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        { 
            //Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
            float offsetU = input.TexCoord.x - (2 * dx) + (j * dx);
            float offsetV = input.TexCoord.y + (2 * dy) - (i * dy);
	        //Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
            if (offsetU > 0 && offsetU < 1 && offsetV > 0 && offsetV < 1)
            {
                color += gTexture.Sample(samPoint, float2(offsetU, offsetV));
                ++totalPass;
            }
        }
    }
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
    color = color / totalPass;
    color.a = 1.0f;
	// Step 5: return the final color
    return color;
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetRasterizerState(BackCulling);
        SetDepthStencilState(EnableDepthWriting, 0);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}