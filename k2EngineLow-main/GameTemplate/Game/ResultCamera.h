#pragma once

class ResultCamera : public IGameObject
{
public:
	ResultCamera();
	~ResultCamera();
	bool Start();
	void Update();

	void SetTarget(const Vector3& targetPos);

private:
	Vector3 pos = { 0.0f,0.0f,30.0f };
	Vector3 target = Vector3::Zero;
};

