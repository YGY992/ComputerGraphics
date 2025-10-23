// DEFINES //
#define NUM_LIGHTS 4

// GLOBALS //
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding;
};

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 globalDiffuseColor; //방향광 (diffuse)
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    
    float4 lightPosition[NUM_LIGHTS];      // 점광원 위치(Light Position)
    float4 pointDiffuseColor[NUM_LIGHTS];  // 점광원 diffuse색상 (Diffuse Light Color)
};

//cbuffer LightPositionBuffer
//{
//    float4 lightPosition[NUM_LIGHTS];
//};



Texture2D shaderTexture;
SamplerState SampleType;

// TYPEDEFS //
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
    //float3 lightDir[NUM_LIGHTS] : TEXCOORD2; // PointLight Direction
    float3 lightDir0 : TEXCOORD2;
    float3 lightDir1 : TEXCOORD3;
    float3 lightDir2 : TEXCOORD4;
    float3 lightDir3 : TEXCOORD5;
    
    //float3 wolrdPos : TEXCOORD6;

};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    // position → homogeneous
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
   
    //Wolrd, View, Porojection변환 
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    //Texture
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    //Normal -> wolrdSpace
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    //위에서 바꾼 값을 정규화
    output.normal = normalize(output.normal);
    
    // 아래처럼 합쳐도 무방
    //output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

	// Calculate the position of the vertex in the world.
    // 현재 버텍스의 위치를 월드 좌표계로 변환
    worldPosition = mul(input.position, worldMatrix);

    // 각 점광원 방향 계산
    //[unroll]
    //for (int i = 0; i < NUM_LIGHTS; i++)
    //{
    //    output.lightDir[i].xyz = lightPosition[i].xyz - worldPosition.xyz;
    //    //output.lightDir[i] = normalize(output.lightDir[i]);
    //}
    output.lightDir0.xyz = lightPosition[0].xyz - worldPosition.xyz;
    output.lightDir1.xyz = lightPosition[1].xyz - worldPosition.xyz;
    output.lightDir2.xyz = lightPosition[2].xyz - worldPosition.xyz;
    output.lightDir3.xyz = lightPosition[3].xyz - worldPosition.xyz;
    //output.worldPos = worldPosition.xyz;

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
    
    return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{

	float4 textureColor;
	float3 lightDir;
    //float4 color = ambientColor;
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 reflection;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 p_colors[4];
    float p_Intensity[4];
    float p_distance[4];
    float3 p_lightDir[4];
    
    //p_lightDir[0] = input.lightDir[0];
    //p_lightDir[1] = input.lightDir[1];
    //p_lightDir[2] = input.lightDir[2];
    //p_lightDir[3] = input.lightDir[3];
    p_lightDir[0] = input.lightDir0;
    p_lightDir[1] = input.lightDir1;
    p_lightDir[2] = input.lightDir2;
    p_lightDir[3] = input.lightDir3;
       
    
    for (int i = 0; i < 3; i++)
    {
        //p_distance[i + 1] = dot(input.lightDir[i + 1], input.lightDir[i + 1]);
        p_distance[i + 1] = dot(p_lightDir[i + 1], p_lightDir[i + 1]);
        //p_Intensity[i + 1] = max(dot(normalize(input.lightDir[i + 1]), input.normal), 0.0f);
        p_Intensity[i + 1] = max(dot(normalize(p_lightDir[i + 1]), input.normal), 0.0f);
        p_colors[i + 1] = pointDiffuseColor[i + 1] / (p_distance[i + 1] * 0.1f);
        p_colors[i + 1] = saturate(p_colors[i + 1] * p_Intensity[i + 1]);
        //colors[i] = pointDiffuseColor[i] * lightIntensity[i];
    }
    
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    // 텍스처 샘플
	textureColor = shaderTexture.Sample(SampleType, input.tex);
    color = ambientColor;
    // 방향광 계산
    lightDir = normalize(-lightDirection); //픽셀로 오는 빛의 방향만을 구함
    float dirIntensity = saturate(dot(input.normal, lightDir)); //픽셀에 도달하는 빛의 세기 계산
    if (dirIntensity > 0.0f) //빛을 받는다면
    {
        color += globalDiffuseColor * dirIntensity; //방향광(diffuse) 계산
        color = saturate(color);
        reflection = normalize(2 * dirIntensity * input.normal - lightDir); //반사벡터 계산 (빛이 표면에 반사되는 방향을 구함) -> 스페큘러를 위해서
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower) * specularColor; //하이라이트 (specular)계산
        //기존의 계산식에서 specularColor 추가.
    }
    
    color *= textureColor;
    color += p_colors[1] + p_colors[2] + p_colors[3];
    color = saturate(color + specular);

    //if (p_distance[1].x > 100.0f || p_distance[2].x > 100.0f || p_distance[3].x > 100.0f)
    //{
    //    color = float4(0, 0, 0, 0);
    //}
    
    //if (lightPosition[1].x <= -200.0f)
    //{
    //    color = pointDiffuseColor[1];
    //}
    
    return color;
}