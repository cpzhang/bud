//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 g_mWorldViewProjection;	// World * View * Projection matrix
float4x4 g_mWorld;	// World
float3   g_vLightPosition;
//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TexCoord   : TEXCOORD0;  // vertex texture coordinate
    float  Diffuse    : COLOR0;
};


//-----------------------------------------------------------------------------
// Name: RenderSceneVS
// Type: Vertex shader                                      
// Desc: This shader computes standard transform and lighting
//-----------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION
			 ,float2 vTexCoord : TEXCOORD0
			 ,float3 vNormal : NORMAL0
    )
{
    VS_OUTPUT Output;
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.TexCoord = vTexCoord;
    float4 worldPosition = mul(vPos, g_mWorld);
    float3 nLight = normalize(g_vLightPosition - worldPosition.xyz);
    float3 nNormal = normalize(vNormal);
    Output.Diffuse = max(0.0, dot(nLight, nNormal));
    return Output;
}
texture g_MeshTexture;              // Color texture for mesh
sampler MeshTextureSampler =
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = tex2D(MeshTextureSampler, In.TexCoord) * In.Diffuse * 2.0;

    return Output;
}

//-----------------------------------------------------------------------------
// Name: RenderScene
// Type: Technique                                     
// Desc: Renders scene to render target
//-----------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS();
	/* The default value is D3DCULL_CCW. */
	CullMode = None;
        PixelShader  = compile ps_2_0 RenderScenePS();
        /* The default value is D3DSHADE_GOURAUD */
	/* ShadeMode = Flat; */
    }
    pass PRecover
    {
	/* CullMode = CCW; */
    }
}