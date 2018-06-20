// TextureMapping.fx

// �ѹ��� ������ �༮�� �޾ƿ͵� ��
float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

// ���̴� ��ü���� ����ص� ��
// �ܺΰ��� ��� flaot4�� ���Ե�
float2 maxFrame;
float2 currentFrame;

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

// ���ؽ� ���̴��� ��ȯ�ɶ� �� �������� �־����
struct VS_OUTPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

	// postion float4 �� �ִ� ������ 4x4 matrix�� ���ؾߵǼ�
	// �ٸ� ������� 3x3 matrix�� �ٲٸ� �Ǳ� �� (���Ҷ� �ʿ���°� ����)
	// �� ��ȯ�� c�� �Ȱ��� (float3x3) �̷������� �ϸ� ��
	// float3 -> float4 �� ���� ����ȯ�� �ȵǰ�
	// float4 temp = float4(input.position, 1.0f);

	// ����� �������ϰ� ���� �� �� ���� �׷���� w�� �ʱ�ȭ
    input.position.w = 1.0f;
	
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

	// uv ��ǥ�� �ȼ� ���̴����� ����
    output.uv = input.uv;

    return output;
}

// �ܺο��� �ؽ�ó �������� ���� ���� �ʿ�
texture tex;

// �ؽ�ó�� uv ��ǥ�� �Ѿ���� ��� ������� ����
// �ؽ�ó ũ�⺸�� ũ�ų� ������ ��� ó������ ����
// �ܺο��� �����Ѱ� ������ �� �� �ְ�
// ���ο��� �����ؼ� �� ���� ����
// LINEAR �׷���ī�忡 ���� �� �ִ°� �ָ��ִ°� �� �����߸���
sampler2D tex_sampler = sampler_state
{
    Texture = tex;
	// �ؽ�ó�� ����������� Ŀ�� �� ó��
    MAGFILTER = LINEAR;
	// �ؽ�ó�� ����������� �۾��� �� ó��
    MINFILTER = LINEAR;
	// �ؽ�ó�� ����������� ���� �� ó��
    MIPFILTER = LINEAR;
};

// �ȼ� ���̴������� �������� ��� ��
struct PS_INPUT
{
    float2 uv : TEXCOORD0;
};

// ��ȯ�Ǵ� ���� ������ color �ؽ�ó�� ���� �÷����� ��ȯ�ǾߵǼ�
float4 PS(PS_INPUT input) : COLOR0
{
    float2 temp;
    temp.x = input.uv.x / maxFrame.x + currentFrame.x / maxFrame.x;
    temp.y = input.uv.y / maxFrame.y + currentFrame.y / maxFrame.y;
	
	// ��ü ������ �÷��� �����ص� �� RGBA ��
    return tex2D(tex_sampler, temp) * float4(1.0f, 0, 0, 0.5f);
}

technique MyShader
{
    pass Pass_0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}