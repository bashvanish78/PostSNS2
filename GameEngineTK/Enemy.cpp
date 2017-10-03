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
	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/caterpillar.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");
	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_CANNON2].LoadModel(L"Resources/Cannon.cmo");
	//m_Obj[PARTS_BULLET].LoadModel(L"Resources/Head.cmo");

	//�e�q�֌W�̍\�z(�q�p�[���ɐe��ݒ�)
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD2].SetObjParent(&m_Obj[PARTS_ENGINE]);
	m_Obj[PARTS_CANNON].SetObjParent(&m_Obj[PARTS_HEAD2]);
	m_Obj[PARTS_CANNON2].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̂���)��ݒ�
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

	//�S�g�p�̂����蔻��m�[�h�̐ݒ������
	{
		m_CollisionNodeBody.Initialize();
		//�e�I�u�W�F�N�g��ݒ�
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

	//�p�[�c�M�~�b�N
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
	//����I�ɐi�s������ς���
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		//�ڕW�p�x��ύX
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}

	{	
		//���@�̊p�x����]
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

		//���
		rot.y += angle*0.01f;
		SetRot(rot);

	}
	// �@�̂������Ă�������ɐi�ޏ���
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.05f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);
	}

	Calc();
	

}

void Enemy::Draw()
{
	//�v���C���[�̕`��
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
