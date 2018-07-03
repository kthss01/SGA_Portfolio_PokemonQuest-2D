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
	float time = 0.3f;
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

	float PlayTime;
	bool isReverse = false;
public:
	AnimationClip();
	~AnimationClip();

	void Play();
	void Play(int direction, int directionSize = 8);

	void Update(AniRepeatType playType);
	void Update(int direction, int directionSize = 8, AniRepeatType playType = AniRepeatType_Loop);

	void PushAnimationData(AnimationData data);
	int GetAnimationDataSize() { return aniClip.size(); }
	AnimationData GetCurrentData();
};

