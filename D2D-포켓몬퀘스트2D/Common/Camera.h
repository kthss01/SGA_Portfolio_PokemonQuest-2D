#pragma once

#include "Transform.h"

class Camera : public Transform
{
private:
	Matrix matView;
	Matrix matProjection;
	Matrix matViewProjection;

public:
	Camera();
	~Camera();

	void UpdateMatrix();
	// Device�� view�� matrix ����, ī�޶� ����ȭ�鿡 ���ڴ�.
	void UpdateCamToDevice();

	Matrix GetViewMatrix() { return matView; }
	Matrix GetProjection() { return matProjection; }
	Matrix GetViewProjection() { return matViewProjection; }
};

