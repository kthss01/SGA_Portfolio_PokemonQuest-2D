#pragma once
// Ư�� �ִϸ��̼� �ൿ�� ���������� �����ؼ� animation clip�̶�� ��

// �ִϸ��̼��� �����ϴ� ���
enum AniRepeatType {
	AniRepeatType_END = 0, // �ѹ��� ����
	AniRepeatType_Loop,		// ����ؼ� ����
	AniRepeatType_Reverse	// ó������ ������ ���㿡 �������� ����
};

// AnimationClip���� �� ������
struct AnimationData {
	wstring keyName;	// ���� ���° ���������� ©������ ����1, ����2 ...
	Vector2 currentFrame;	// �� data�� ���� ������ ��
	Vector2 maxFrame;	// �̹����� ���ؼ� ������ ¥������
						// ���� �� �ִϸ��̼ǿ��� ���� �������� �Ѿ�µ� �ɸ��� �ð�
						// frameRate�� ������ ��������
	float time = 0.2f;
};

// ���� �ϳ� ������ ���� �̷��� �� �ִ°�
class AnimationClip
{
private:
	wstring keyName;	// ����, ����, ���� ������
	float frameRate;	// ���� �ʴ� ��� �ִϸ��̼��� ������ ������
	vector<AnimationData> aniClip;
	wstring textureName;

	int index;

	bool isPlay = false;

	float PlayTime;
	bool isReverse = false;
public:
	AnimationClip();
	~AnimationClip();

	void Play();

	bool IsPlay() { return isPlay; }

	void Update(AniRepeatType playType);

	void PushAnimationData(AnimationData data);
	AnimationData GetCurrentData();
};

