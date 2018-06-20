// BaseColor.fx

// 1. �ڵ��ϼ������ ����.
// 2. ��Ż�� �˻簡 ����.
// 3. hlsl - high level shader language �̳༮�� Ȯ���ڷ� �� �� ����
// .hlsl(�Ϲ����� Ȯ����, ���� ������ �𸣸� �̳༮����) 
// .fx(ȿ��) 
// .ps(�ȼ� ���̴�) 
// .vs(���ؽ� ���̴�)

// 4. ������ �޶���  -> Vector2, Matrix ���� ���� ����
//					-> float2, float4x4 (Vector3 float3��)

/*
	���̴� ������ ����ϴ� �Լ����� ���� �����ϱ�� ��
*/

// ��������
// �ܺο��� ������ �� (���� ������ ��), ���̴� ������ �� �ʿ��� �� ����

// �ܺο��� �������� ��Ʈ������ �˻��ؼ� �ְ� �Ǿ�����
// �Ķ���ʹ� ���� string���� �˻�
// ex) ("a", 10.0f) �̷�������
//		("b", D3DXVECTOR4(0,0,0,0))
// ("a", 5.0f") // �ߺ��Ǹ� ������ �༮�� �� 
// string -> 

// ��Ÿ ����
float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

// vs ���ؽ� ���̴�
// ���ؽ� ���̴��� ���� ��
// ���ؽ� ���ڸ�ŭ ���ؽ� ���̴��� ���ư�
// ���ؽ� INPUT�� �ܺο��� �� ��
struct VS_INPUT
{
	// FVF �� ���� (��ǻ�Ϳ� POSITION�� �� �Ŷ� �˷��ִ°�)
    float4 position : POSITION; // POSITION�� �ø�ƽ 
    float4 color : COLOR0;
};

// ps �ȼ� ���̴�
// �ȼ����� ��
// ���ؽ� ���̴����� ���� �༮�� �ȼ� ���̴����� �� ����ü��
struct PS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

// �Լ� ����Ű� ���� �Լ� �����ϴ°� �ƴ϶�
// ���ؽ� ����ŭ �����
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

    output.color = input.color;
    return output; // ���� ���������൵ �ڵ����� �ȼ� ���̴��� ��
}

// ���ؽ��� ȭ��� �����ġ�ϴ��� ���� �� �ȿ� �ȼ����� ��������
// �ȿ��� �ȼ� ����ŭ ����� 
// ex)100 x 100 �϶� 10004 �� �����
// �簢�� vertex 4���̶� VS 4�� ���� 1000 x 1000 �鸸�� ����
float4 PS(PS_INPUT input) : COLOR0 // ��ȯ�Ǵ� float4 � �༮���� ������
{
    return input.color;
}

// ��ũ��
// ���̴� ��� ��Ȳ�� ���� ������ ���� ������
technique MyShader
{
	// pass ������ ���� �� �� �̸��� ������� pass ������ �༮���� ���ư�
    pass P0
    {
		// vs_3_0 , ps_3_0 compile ���� ���ݱ��� ���� ���� 5.1�����ΰ� ����
		// ���� �ö� ���� ���̴� �ϳ��� ������ ���� �־ ����� �� �ִ� �ɷ��� �ٸ�
		// 3_0 �ѹ��� ����ְ� ����Ҷ� ���� ����
		// 5_0 �ʸ��� �̻�
		// ���� ������ ���̴� ������ ����ؾߵ�
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}

// vector���� ������ vector4 �� �־��־�� �� vector2 ������
// b �༮ x,y �� ������ float 2,3,4 �� vector4�� �־����
// ex) ("b", D3DXVECTOR4(x,y,0,0)
//float a;
//float2 b;
//float4x4 c;
// �̷��� ���� Ŭ�������� �˻��� �� a or b or c�� �˻��ؾߵ�