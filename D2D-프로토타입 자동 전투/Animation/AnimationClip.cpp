#include "stdafx.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip()
{
	index = 0;
}


AnimationClip::~AnimationClip()
{
}

void AnimationClip::Play()
{
	index = 0;

	// 현재 시간을 받아서 사용
	PlayTime = FRAME->GetWorldTime();
}

void AnimationClip::Play(int direction, int directionSize)
{
	index = direction * aniClip[0].maxFrame.x / directionSize;

	PlayTime = FRAME->GetWorldTime();
}

void AnimationClip::Update(AniRepeatType playType)
{
	float currentTime = FRAME->GetWorldTime();
	AnimationData clip = aniClip[index];

	if (currentTime - PlayTime > clip.time) {
		switch (playType)
		{
		case AniRepeatType_END:
			index++;
			if (index >= aniClip.size() - 1)
				index = aniClip.size() - 1;
			break;
		case AniRepeatType_Loop:
			index++;
			index %= aniClip.size();
			break;
		case AniRepeatType_Reverse:
			if (isReverse == false) {
				index++;
				if (index >= aniClip.size() - 1)
					isReverse = true;
			}
			else {
				index--;
				if (index <= 0)
					isReverse = false;
			}
			break;
		}
		PlayTime = FRAME->GetWorldTime();
	}
}

void AnimationClip::Update(int direction, int directionSize, AniRepeatType playType)
{
	float currentTime = FRAME->GetWorldTime();
	AnimationData clip = aniClip[index];

	if (currentTime - PlayTime > clip.time) {
		switch (playType)
		{
		case AniRepeatType_END:
			index++;
			if (index >= aniClip.size() - 1)
				index = aniClip.size() - 1;
			break;
		case AniRepeatType_Loop:
			index++;
			//index %= aniClip.size();
			if (index >= clip.maxFrame.x / directionSize * (direction + 1)) {
				index = clip.maxFrame.x / directionSize * direction;
			}
			break;
		case AniRepeatType_Reverse:
			if (isReverse == false) {
				index++;
				if (index >= aniClip.size() - 1)
					isReverse = true;
			}
			else {
				index--;
				if (index <= 0)
					isReverse = false;
			}
			break;
		}
		PlayTime = FRAME->GetWorldTime();
	}
}

void AnimationClip::PushAnimationData(AnimationData data)
{
	aniClip.push_back(data);
}

AnimationData AnimationClip::GetCurrentData()
{
	return aniClip[index];
}
