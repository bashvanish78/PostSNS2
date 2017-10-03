#include "pch.h"
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy()
{

	m_debug = false;

}

void Enemy::Initialize()
{
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/caterpillar.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");
	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_CANNON2].LoadModel(L"Resources/Cannon.cmo");
	//m_Obj[PARTS_BULLET].LoadModel(L"Resources/Head.cmo");

	//親子関係の構築(子パールに親を設定)
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD2].SetObjParent(&m_Obj[PARTS_ENGINE]);
	m_Obj[PARTS_CANNON].SetObjParent(&m_Obj[PARTS_HEAD2]);
	m_Obj[PARTS_CANNON2].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//子パーツの親からのオフセット(座標のずれ)を設定
	m_Obj[PARTS_ENGINE].SetTrans(Vector3(0.0f, 0.2f, 0.0f));
	m_Obj[PARTS_HEAD].SetTrans(Vector3(0.0f, 0.6f, -0.45f));
	m_Obj[PARTS_HEAD2].SetTrans(Vector3(0.0f, 1.0f, 0.2f));
	m_Obj[PARTS_CANNON].SetTrans(Vector3(0.0f, 0.4f, 0.0f));
	m_Obj[PARTS_CANNON2].SetTrans(Vector3(0.0f, 0.6f, 0.0f));

	//m_Obj[PARTS_BULLET].SetTrans(m_Obj[PARTS_CANNON].GetTrans());

	m_Obj[PARTS_CANNON2].SetScale(m_Obj[PARTS_CANNON2].GetScale() * -1);

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

	//全身用のあたり判定ノードの設定をする
	{
		m_CollisionNodeBody.Initialize();
		//親オブジェクトを設定
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_HEAD2]);

		m_CollisionNodeBody.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBody.SetLocalRadius(0.6f);
	}

}


Enemy::~Enemy()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Finalize();
	}
}

void Enemy::Update()
{

	//パーツギミック
	{
		m_cycle += 0.1f;
		float scale = 1.6f + sinf(m_cycle);
		m_Obj[PARTS_HEAD2].SetScale(Vector3(scale, scale, scale));

		float angle = m_Obj[PARTS_CANNON].GetRot().y;
		m_Obj[PARTS_CANNON].SetRot(Vector3(0.0f, angle + 0.03f, 0.0f));
		float angle2 = m_Obj[PARTS_CANNON2].GetRot().y;
		m_Obj[PARTS_CANNON2].SetRot(Vector3(0.0f, angle2 + 0.03f, 0.0f));

		//float angle = m_Obj[PARTS_CANNON].GetRot().x;
		//m_Obj[PARTS_HEAD].SetRot(Vector3(angle + 0.03f, 0.0f, 0.0f));
		m_Obj[PARTS_HEAD].SetScale(Vector3(1, scale, 1));

		float angleengine = m_Obj[PARTS_CANNON].GetRot().x + sinf(m_cycle);
		m_Obj[PARTS_ENGINE].SetRot(Vector3(angleengine, 0.0f, 0.0f));

	}
	//定期的に進行方向を変える
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

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
	// 機体が向いている方向に進む処理
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.05f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);
	}

	Calc();
	

}

void Enemy::Draw()
{
	//プレイヤーの描画
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
	if (m_debug)m_CollisionNodeBody.Draw();
}

void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	m_CollisionNodeBody.Update();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	return m_Obj[0].GetTrans();
}

const DirectX::SimpleMath::Matrix& Enemy::GetWorld()
{
	return m_Obj[0].GetWorld();
}

const DirectX::SimpleMath::Vector3& Enemy::GetScale()
{
	return m_Obj[0].GetScale();
}

const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	return m_Obj[0].GetRot();
}

void Enemy::SetScale(DirectX::SimpleMath::Vector3& scale)
{
	m_Obj[0].SetScale(scale);
}

void Enemy::ChangeDebug()
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

void Enemy::SetRot(DirectX::SimpleMath::Vector3& rot)
{
	m_Obj[0].SetRot(rot);
}

void Enemy::SetTrans(DirectX::SimpleMath::Vector3& trans)
{
	m_Obj[0].SetTrans(trans);
}
