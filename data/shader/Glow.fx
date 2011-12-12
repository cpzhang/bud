//--------------------------------------------------------------------------------------
// File: BasicHLSL.fx
//
// The effect file for the BasicHLSL sample.  
// fxc /LD /Od /Zi /T fx_2_0 /Fo glow.fxo glow.fx
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

texture g_MeshTexture;              // Color texture for mesh

float4x4 g_mWorldViewProjection;    // World * View * Projection matrix



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT VSmain( float4 vPos : POSITION, 
                  float2 vTexCoord0 : TEXCOORD0 )
{
    VS_OUTPUT Output;
  
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vPos, g_mWorldViewProjection);
    //
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel 
};

float gGaussianEfficient[49] = {
  0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067,
  0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292, 
  0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117, 
  0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771, 
  0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117, 
  0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292, 
  0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067, 
};

float4 GaussianFilter(VS_OUTPUT In, int gRadius,  float stepU,  float stepV) : COLOR0
{
  float4 result = 0;
  // Lookup mesh texture and modulate it with diffuse
  for(int u = -gRadius; u <= gRadius; u++)
      for(int v = -gRadius; v <= gRadius; v++)
	{
	  result += tex2D(MeshTextureSampler, In.TextureUV + float2(stepU * u, stepV * v)) * gGaussianEfficient[(u + gRadius) * (2 * gRadius + 1) + (v + gRadius)]; 
	}
  return result;
}
//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT PSmain( VS_OUTPUT In , uniform int gRadius, uniform float stepU, uniform float stepV, uniform int iterator) 
{ 
  PS_OUTPUT Output;
  Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV);
  //  float4 blurColor = 0.0f;
  //for(int i = 1; i <= gRadius; ++i)
  // {
  //  float step = 0.1f / pow(2, i);
  //  blurColor += GaussianFilter(In, i, stepU, stepV);
  //}
  //Output.RGBColor += 0.5 * blurColor;
   Output.RGBColor *= Output.RGBColor.w;
  return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique Glow
{
  pass P0
    {          
      VertexShader = compile vs_3_0 VSmain();
      PixelShader  = compile ps_3_0 PSmain(3, 0.05, 0.05, 5);
    }
}
