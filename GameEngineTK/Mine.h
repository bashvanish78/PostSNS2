#include <SimpleMath.h>
#include"Obj3d.h"
#include "CollisionNode.h"


#pragma once
class Mine
{
public:

	//自機パーツ
	enum PARTS
	{
		PARTS_BODY,	//本体

		PARTS_NUM
	};

	//コンストラクタ
	Mine();
	//デストラクタ
	~Mine();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//計算
	void Calc();

	//座標を習得
	const DirectX::SimpleMath::Vector3& GetTrans();
	//回転を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	//スケールを取得
	const DirectX::SimpleMath::Vector3& GetScale();
	//ワールド座標を取得
	const DirectX::SimpleMath::Matrix& GetWorld();

	//座標を設定
	void SetTrans(DirectX::SimpleMath::Vector3& trans);
	//回転を設定
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//スケールを設定
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//全身用のあたり判定の取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//デバッグ状態変更
	void ChangeDebug();

private:

	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//発射してから消えるまで
	int shootcount;
	//消える時間
	int deletecount;
	//弾の角度
	float bulletangle;
	float cannonangle;

	float m_DistAngle;
	int m_timer;

	//全身用あたり判定オブジェクト
	SphereNode m_CollisionNodeBody;

	//デバッグ
	bool m_debug;
};

