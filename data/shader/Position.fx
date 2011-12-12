//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 g_mWorldViewProjection;	// World * View * Projection matrix

//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
};


//-----------------------------------------------------------------------------
// Name: RenderSceneVS
// Type: Vertex shader                                      
// Desc: This shader computes standard transform and lighting
//-----------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION)
{
    VS_OUTPUT Output;
    // Transform the position from object space to homogeneous projection space
    Output.Position = vPos;
    
    return Output;    
}


//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//-----------------------------------------------------------------------------
// Name: RenderScenePS                                        
// Type: Pixel shader
// Desc: This shader outputs the pixel's color 
//-----------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = float4(1.0, 0.0, 0.0, 0.0);

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
        PixelShader  = compile ps_2_0 RenderScenePS();
    }
}
