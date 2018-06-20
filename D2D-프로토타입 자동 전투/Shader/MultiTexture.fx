// MutliTexture.fx

// texture �ѷ� ������
// ��¿� texture (�÷�)
// �����Ϳ� texture (normal map, height map, light map)

float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

    output.uv = input.uv;

    return output;
}

// tex1 �� <> �ϸ� �̸� �� ���� �� �� ���� ��ζ���� ũ������
// texture tex1<>

texture tex1;
sampler2D tex1_sampler = sampler_state
{
    Texture = tex1;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};

texture tex2;
sampler2D tex2_sampler = sampler_state
{
    Texture = tex2;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};

// 0�� Ÿ�� ��ȣ�� �ǹ��ϴ°�
// �׳� COLOR Ÿ�� �ϳ��� ���� �� ����
float4 PS(PS_INPUT input) : COLOR0
{
    float4 texColor1 = tex2D(tex1_sampler, input.uv);
    float4 texColor2 = tex2D(tex2_sampler, input.uv);

    //if(texColor1.x > 0.8f) 
    //    texColor1.x = 0.0f;
    //if(texColor1.y > 0.8f) 
    //    texColor1.y = 0.0f;
    //if (texColor1.z > 0.8f)
    //    texColor1.z = 0.0f;

    //// ���°�
    //return texColor1 * texColor2;
    // ���ϴ°Ŵ�
    // �ִ밪�� 1.0f �̱� ������ �Ѿ�°� ������� ����

    //float gray = (texColor2.x + texColor2.y + texColor2.z) / 3;
    //float4 finalColor = float4(gray, gray, gray, 1.0f);
    //return texColor1 * finalColor;

    texColor2.x = 1 - texColor2.x;
    texColor2.y = 1 - texColor2.y;
    texColor2.z = 1 - texColor2.z;

    return texColor2;
}

technique MyShader
{
    pass Pass_0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}