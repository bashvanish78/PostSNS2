#include "pch.h"
#include "Mine.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Mine::Mine()
{

	m_debug = false;

}

void Mine::Initialize()
{
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_BODY].LoadModel(L"Resources/Mine.cmo");

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
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_BODY]);

		m_CollisionNodeBody.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBody.SetLocalRadius(0.6f);
	}

}


Mine::~Mine()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Finalize();
	}
}

void Mine::Update()
{
	//パーツギミック
	{
		m_Obj[PARTS_BODY].SetRot(Vector3(m_Obj[PARTS_BODY].GetRot().x, m_Obj[PARTS_BODY].GetRot().y+0.1f, m_Obj[PARTS_BODY].GetRot().z));
	}
	Calc();
}

void Mine::Draw()
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

void Mine::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	m_CollisionNodeBody.Update();
}

const DirectX::SimpleMath::Vector3& Mine::GetTrans()
{
	return m_Obj[0].GetTrans();
}

const DirectX::SimpleMath::Matrix& Mine::GetWorld()
{
	return m_Obj[0].GetWorld();
}

const DirectX::SimpleMath::Vector3& Mine::GetScale()
{
	return m_Obj[0].GetScale();
}

const DirectX::SimpleMath::Vector3& Mine::GetRot()
{
	return m_Obj[0].GetRot();
}

void Mine::SetScale(DirectX::SimpleMath::Vector3& scale)
{
	m_Obj[0].SetScale(scale);
}

void Mine::ChangeDebug()
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

void Mine::SetRot(DirectX::SimpleMath::Vector3& rot)
{
	m_Obj[0].SetRot(rot);
}

void Mine::SetTrans(DirectX::SimpleMath::Vector3& trans)
{
	m_Obj[0].SetTrans(trans);
}
