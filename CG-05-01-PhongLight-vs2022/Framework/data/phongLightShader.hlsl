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
	float4 globalDiffuseColor; //���Ɽ (diffuse)
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    
    float4 lightPosition[NUM_LIGHTS];      // ������ ��ġ(Light Position)
    float4 pointDiffuseColor[NUM_LIGHTS];  // ������ diffuse���� (Diffuse Light Color)
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
    // position �� homogeneous
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
   
    //Wolrd, View, Porojection��ȯ 
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
    //������ �ٲ� ���� ����ȭ
    output.normal = normalize(output.normal);
    
    // �Ʒ�ó�� ���ĵ� ����
    //output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

	// Calculate the position of the vertex in the world.
    // ���� ���ؽ��� ��ġ�� ���� ��ǥ��� ��ȯ
    worldPosition = mul(input.position, worldMatrix);

    // �� ������ ���� ���
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
    // �ؽ�ó ����
	textureColor = shaderTexture.Sample(SampleType, input.tex);
    color = ambientColor;
    // ���Ɽ ���
    lightDir = normalize(-lightDirection); //�ȼ��� ���� ���� ���⸸�� ����
    float dirIntensity = saturate(dot(input.normal, lightDir)); //�ȼ��� �����ϴ� ���� ���� ���
    if (dirIntensity > 0.0f) //���� �޴´ٸ�
    {
        color += globalDiffuseColor * dirIntensity; //���Ɽ(diffuse) ���
        color = saturate(color);
        reflection = normalize(2 * dirIntensity * input.normal - lightDir); //�ݻ纤�� ��� (���� ǥ�鿡 �ݻ�Ǵ� ������ ����) -> ����ŧ���� ���ؼ�
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower) * specularColor; //���̶���Ʈ (specular)���
        //������ ���Ŀ��� specularColor �߰�.
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