#include <SimpleMath.h>
#include"Obj3d.h"
#include "CollisionNode.h"

#pragma once
class Boss
{
public:

	//自機パーツ
	enum PARTS
	{
		PARTS_HEAD2,	//頭2
		PARTS_BULLET,

		PARTS_NUM
	};

	//コンストラクタ
	Boss();
	//デストラクタ
	~Boss();
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
	void Damage();
	//回転を設定
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//スケールを設定
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//弾丸用のあたり判定の取得
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	//全身用のあたり判定の取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//デバッグ状態変更
	void ChangeDebug();

	const int& GetHP() { return m_HP; }
	void SetHP(const int& HP) { m_HP = HP; }

	const int& GetDefaultHP() { return m_defaultHP; }

	//無敵状態
	void SetInvincibleState(const bool& invinciblestate) { m_invincibleState = invinciblestate; }
	const bool& GetInvincibleState() { return m_invincibleState; }

	const int& GetInvincibleCount() { return m_invincibleCount; }

	//弾を発射している状態か取得
	const bool GetFireFlag();

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

	//弾丸用あたり判定オブジェクト
	SphereNode m_CollisionNodeBullet;
	//全身用あたり判定オブジェクト
	SphereNode m_CollisionNodeBody;

	//デバッグ
	bool m_debug;

	const int m_defaultHP = 5;
	int m_HP;

	const int m_invincibleTime = 60;
	int m_invincibleCount;

	bool m_invincibleState;

	void UpAndDown();
	int m_UpAndDownCount;
	const int m_UpAndDownTime = 30;
	bool m_Ystate;

	void FireBullet();
	//発射している状態
	bool m_FireFlag;

	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	void BossBullet();
	//弾丸を戻す
	void ResetBullet();

	int m_shootcount;
	//弾が消える時間
	const int m_deletecount = 30;

	int m_startShootCount;
	const int m_startShootTime = 60;

};
