#include <SimpleMath.h>
#include "Obj3d.h"
#include "CollisionNode.h"
#include <Keyboard.h>

#pragma once
class Player
{
public:
	//重力加速度
	const float GRAVITY_ACC = 0.03f;
	//ジャンプ初速
	const float JUMP_SPEED_FIRST = 0.5f;
	//落下速度制限
	const float JUMP_SPEED_MAX = 0.3f;

	//自機パーツ
	enum PARTS
	{
		PARTS_FIGHTER,//機体
		PARTS_R_WING,//右翼
		PARTS_L_WING,//左翼
		PARTS_BULLET,//弾
		PARTS_BOOST,//ブースト

		PARTS_NUM
	};

	Player(DirectX::Keyboard* key);
	void Initialize();
	~Player();
	void Update();
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
	//速度取得
	const DirectX::SimpleMath::Vector3& GetVelocity();
	//弾を発射している状態か取得
	const bool GetFireFlag();


	//座標を設定
	void SetTrans(DirectX::SimpleMath::Vector3& trans);
	//回転を設定
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//スケールを設定
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//弾丸を発射
	void FireBullet();
	//弾丸を戻す
	void ResetBullet();

	//弾丸用のあたり判定の取得
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	//全身用のあたり判定の取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//死んだとき
	void SetDeathState(const bool& deathstate) { m_deathstate = deathstate; }

	const bool& GetDeathState() { return m_deathstate; }

	//死亡回数のカウント
	void PlusDeathCount() { m_defeatCount++; }

	//リスポーンカウント初期化
	void ResetRespawnCount() { m_respawnCount = 0; }

	//デバッグ状態変更
	void ChangeDebug();

	void StartJump();

	void StartFall();

	void StopJump();

	//羽の状態(0,無傷.1,左翼.2,右翼)
	void SetWingState(const int& wingstate) { m_wingState = wingstate; }
	const int& GetWingState() { return m_wingState; }

	//無敵状態
	void SetInvincibleState(const bool& invinciblestate) { m_invincibleState = invinciblestate; }
	const bool& GetInvincibleState() { return m_invincibleState; }

	const int& GetInvincibleCount() { return m_invincibleCount; }

	void Damage();

	void SetGameSceneResume(const bool resume) { m_gamesceneResume = resume; }

	int GetShootTimeCount() { return m_shootTimeCount; }
	int GetDamageCount() { return m_damageCount; }
	int GetDefeatCount() { return m_defeatCount; }

	void PlayerDataReset();

private:
	//パーツギミック
	void PartsGimic();
	void PlayerMove(DirectX::Keyboard::State kb);
	void PlayerBullet();

	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//弾を発射してからのカウント
	int m_shootcount;
	//弾が消える時間
	const int m_deletecount = 30;

	//弾の角度
	float bulletangle;
	float cannonangle;

	//発射している状態
	bool m_FireFlag;

	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//弾丸用あたり判定オブジェクト
	SphereNode m_CollisionNodeBullet;
	//全身用あたり判定オブジェクト
	SphereNode m_CollisionNodeBody;
	
	//速度
	DirectX::SimpleMath::Vector3 m_Velocity;
	//落下中フラグ
	bool m_isJump;

	//デバッグ
	bool m_debug;

	//ブーストのモデル拡縮
	int m_boostModelCount;
	const int m_boostModelTime = 6;
	bool m_boostModelState;

	bool m_deathstate;

	const int m_respawnTime = 90;
	int m_respawnCount;

	int m_wingState;

	const int m_invincibleTime = 60;
	int m_invincibleCount;

	bool m_invincibleState;

	bool m_gamesceneResume;

	int m_shootTimeCount;
	int m_defeatCount;
	int m_damageCount;
};

