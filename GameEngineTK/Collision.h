/// <summary>
/// 衝突判定ライブラリ
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

//球
class Sphere
{
public:
	//中心座標
	DirectX::SimpleMath::Vector3 Center;
	//半径
	float Radius;
	//コンストラクタ
	Sphere()
	{
		//デフォルトの半径の大きさ1m
		Radius = 1.0f;
	}
};

//線分
class Segment
{
public:
	//始点座標
	DirectX::SimpleMath::Vector3 Start;
	//終点座標
	DirectX::SimpleMath::Vector3 End;
};

//カプセル
class Capsule
{
public:
	//芯線
	Segment Segment;
	//半径
	float Radius;
	//コンストラクタ
	Capsule()
	{
		Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		Radius = 1.0f;
	}
};

//三角形
class Triangle
{
public:
	//頂点座標
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	//法線ベクトル
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
