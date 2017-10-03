/// <summary>
/// �Փ˔��胉�C�u����
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

//��
class Sphere
{
public:
	//���S���W
	DirectX::SimpleMath::Vector3 Center;
	//���a
	float Radius;
	//�R���X�g���N�^
	Sphere()
	{
		//�f�t�H���g�̔��a�̑傫��1m
		Radius = 1.0f;
	}
};

//����
class Segment
{
public:
	//�n�_���W
	DirectX::SimpleMath::Vector3 Start;
	//�I�_���W
	DirectX::SimpleMath::Vector3 End;
};

//�J�v�Z��
class Capsule
{
public:
	//�c��
	Segment Segment;
	//���a
	float Radius;
	//�R���X�g���N�^
	Capsule()
	{
		Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		Radius = 1.0f;
	}
};

//�O�p�`
class Triangle
{
public:
	//���_���W
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	//�@���x�N�g��
	DirectX::SimpleMath::Vector3 Normal;
};

class Collision
{
public:
	Collision();
	~Collision();
};

bool CheckSphere2Sphere(const Sphere & sphereA, const Sphere & sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

//void ClosestPtPoint2Triangle(const Vector3 & _point, const Triangle & _triangle, Vector3 * _closest);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
