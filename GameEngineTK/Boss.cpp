#include "pch.h"
#include "Boss.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Boss::Boss()
{

	m_debug = false;
	m_FireFlag = false;

}

void Boss::Initialize()
{
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);

	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/PlayerBullet.cmo");

	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//弾は発射してないとき非表示
	m_Obj[PARTS_BULLET].SetDrawState(false);

	m_Obj[PARTS_BULLET].SetScale(Vector3(0.f, 0.f, 0.f));

	m_cycle = 0.0f;

	shootcount = 0;

	deletecount = 60;
	deletecount += 1;

	Vector3 pos;
	pos.x = rand() % 10;
	pos.z = rand() % 10;
	SetTrans(pos);

	m_timer = 60;
	m_DistAngle = 0;

	m_shootcount = 0;

	//弾丸用のあたり判定ノードの設定をする
	{
		m_CollisionNodeBullet.Initialize();
		//親オブジェクトを設定
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BULLET]);

		m_CollisionNodeBullet.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBullet.SetLocalRadius(0.18f);
	}

	//全身用のあたり判定ノードの設定をする
	{
		m_CollisionNodeBody.Initialize();
		//親オブジェクトを設定
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_HEAD2]);

		m_CollisionNodeBody.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBody.SetLocalRadius(0.4f);
	}

	SetHP(GetDefaultHP());

	m_invincibleCount = 0;

	m_invincibleState = false;

	if (rand() % 2)
	{
		m_Ystate = true;
	}
	else
	{
		m_Ystate = false;
	}

	m_UpAndDownCount = 0;

	m_startShootCount = 0;
}


Boss::~Boss()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Finalize();
	}
}

void Boss::Update()
{

	//パーツギミック
	{
		m_cycle += 0.1f;
		float scale = 7.4f + sinf(m_cycle);
		m_Obj[PARTS_HEAD2].SetScale(Vector3(scale, scale, scale));

		//m_Obj[PARTS_HEAD2].SetRot(Vector3(GetRot().x, GetRot().y + 0.1f, GetRot().z));

	}
	//定期的に進行方向を変える
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 12;

		//目標角度を変更
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}

	{
		//自機の角度を回転
		Vector3 rot = GetRot();


		float angle = m_DistAngle - rot.y;

		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}

		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		//補間
		rot.y += angle*0.01f;
		SetRot(rot);

	}

	//上下移動
	UpAndDown();

	m_startShootCount++;
	BossBullet();

	// 機体が向いている方向に進む処理
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.2f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);
	}

	//無敵状態のカウント
	if (GetInvincibleState())
	{
		m_invincibleCount++;
		if (m_invincibleCount > m_invincibleTime)
		{
			m_invincibleCount = 0;
			SetInvincibleState(false);
		}
	}

	Calc();
}

void Boss::Draw()
{
	//無敵カウントが偶数だったら抜ける
	//if (GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0) return;

	//ボスは無敵カウントが偶数だったら表示しない
	if (!(GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0))
	{
		m_Obj[0].Draw();
	}

	//弾
	m_Obj[1].Draw();

	////プレイヤーの描画
	//for (std::vector<Obj3d>::iterator it = m_Obj.begin();
	//	it != m_Obj.end();
	//	it++)
	//{
	//	it->Draw();
	//}

	if (m_debug)m_CollisionNodeBody.Draw();
	if (m_debug) m_CollisionNodeBullet.Draw();
}

void Boss::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBody.Update();
	m_CollisionNodeBullet.Update();
}

const DirectX::SimpleMath::Vector3& Boss::GetTrans()
{
	return m_Obj[0].GetTrans();
}

const DirectX::SimpleMath::Matrix& Boss::GetWorld()
{
	return m_Obj[0].GetWorld();
}

const DirectX::SimpleMath::Vector3& Boss::GetScale()
{
	return m_Obj[0].GetScale();
}

const DirectX::SimpleMath::Vector3& Boss::GetRot()
{
	return m_Obj[0].GetRot();
}

void Boss::SetScale(DirectX::SimpleMath::Vector3& scale)
{
	m_Obj[0].SetScale(scale);
}

void Boss::ChangeDebug()
{
	if (m_debug)
	{
		m_debug = false;
	}
	else
	{
		m_debug = true;
	}

}

const bool Boss::GetFireFlag()
{
	return m_FireFlag;
}

void Boss::UpAndDown()
{
	float spd = 0.3f;

	if (m_Ystate)
	{
		SetTrans(Vector3(GetTrans().x, GetTrans().y + spd , GetTrans().z));
	}
	else
	{
		SetTrans(Vector3(GetTrans().x, GetTrans().y - spd, GetTrans().z));
	}

	m_UpAndDownCount++;

	if (m_UpAndDownCount > m_UpAndDownTime)
	{
		if (m_Ystate)
		{
			m_Ystate = false;
		}
		else
		{
			m_Ystate = true;
		}
		m_UpAndDownCount = 0;
	}
}

void Boss::FireBullet()
{
	//発射中の場合
	if (m_FireFlag) return;

	//発射するパーツのワールド行列を取得
	Matrix worldm = m_Obj[PARTS_HEAD2].GetWorld();
	//抽出した情報を格納
	Vector3 scale;			//ワールドスケーリング
	Quaternion rotation;	//ワールド回転
	Vector3 translation;	//ワールド座標

							//ワールド行列からスケーリング、回転、移動を取り出す
	worldm.Decompose(scale, rotation, translation);

	//スケール固定
	scale = Vector3(25.0f, 25.0f, 25.0f);

	//親パーツから分離、独立させる
	m_Obj[PARTS_BULLET].SetObjParent(nullptr);
	m_Obj[PARTS_BULLET].SetScale(scale);
	m_Obj[PARTS_BULLET].SetRotQ(rotation);
	m_Obj[PARTS_BULLET].SetTrans(translation);
	m_Obj[PARTS_BULLET].SetDrawState(true);

	//弾丸パーツの速度の設定
	m_BulletVel = Vector3(0, 0, -1);
	//パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	m_FireFlag = true;

	//ADX2Le::Play(CRI_CUESHEET_0_LASER2);

	//m_shootCount++;
}

/// <summary>
/// 弾の発射、前進など
/// </summary>
void Boss::BossBullet()
{
	//弾丸パーツの発射
	//発射
	if (m_startShootCount > m_startShootTime)
	{
		if (!m_FireFlag)
		{
			//死んでいたら抜ける
			//if (GetDeathState()) return;
			FireBullet();
		}
		m_startShootCount = 0;
	}

	//弾丸パーツの前進処理、消去処理
	if (m_FireFlag)
	{
		//自機のベクトル
		Vector3 pos = m_Obj[PARTS_BULLET].GetTrans();
		m_Obj[PARTS_BULLET].SetTrans(pos + m_BulletVel);

		m_shootcount++;
		if (m_shootcount > m_deletecount)
		{
			m_shootcount = 0;
			ResetBullet();
		}
	}
}

void Boss::ResetBullet()
{
	//発射中ではない場合
	if (!m_FireFlag) return;

	//場所をリセット
	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_HEAD2]);
	m_Obj[PARTS_BULLET].SetTrans(Vector3(0.0f, 0.0f, 0.0f));
	m_Obj[PARTS_BULLET].SetRot(Vector3(0, 0, 0));
	m_Obj[PARTS_BULLET].SetScale(Vector3(0, 0, 0));
	m_Obj[PARTS_BULLET].SetDrawState(false);

	m_FireFlag = false;

}

void Boss::SetRot(DirectX::SimpleMath::Vector3& rot)
{
	m_Obj[0].SetRot(rot);
}

void Boss::SetTrans(DirectX::SimpleMath::Vector3& trans)
{
	m_Obj[0].SetTrans(trans);
}

void Boss::Damage()
{
	SetHP(GetHP() - 1);
	SetInvincibleState(true);
}