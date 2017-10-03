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
	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);

	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/PlayerBullet.cmo");

	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//�e�͔��˂��ĂȂ��Ƃ���\��
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

	//�e�ۗp�̂����蔻��m�[�h�̐ݒ������
	{
		m_CollisionNodeBullet.Initialize();
		//�e�I�u�W�F�N�g��ݒ�
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BULLET]);

		m_CollisionNodeBullet.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBullet.SetLocalRadius(0.18f);
	}

	//�S�g�p�̂����蔻��m�[�h�̐ݒ������
	{
		m_CollisionNodeBody.Initialize();
		//�e�I�u�W�F�N�g��ݒ�
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

	//�p�[�c�M�~�b�N
	{
		m_cycle += 0.1f;
		float scale = 7.4f + sinf(m_cycle);
		m_Obj[PARTS_HEAD2].SetScale(Vector3(scale, scale, scale));

		//m_Obj[PARTS_HEAD2].SetRot(Vector3(GetRot().x, GetRot().y + 0.1f, GetRot().z));

	}
	//����I�ɐi�s������ς���
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 12;

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

	//�㉺�ړ�
	UpAndDown();

	m_startShootCount++;
	BossBullet();

	// �@�̂������Ă�������ɐi�ޏ���
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.2f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);
	}

	//���G��Ԃ̃J�E���g
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
	//���G�J�E���g�������������甲����
	//if (GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0) return;

	//�{�X�͖��G�J�E���g��������������\�����Ȃ�
	if (!(GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0))
	{
		m_Obj[0].Draw();
	}

	//�e
	m_Obj[1].Draw();

	////�v���C���[�̕`��
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
	//���˒��̏ꍇ
	if (m_FireFlag) return;

	//���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm = m_Obj[PARTS_HEAD2].GetWorld();
	//���o���������i�[
	Vector3 scale;			//���[���h�X�P�[�����O
	Quaternion rotation;	//���[���h��]
	Vector3 translation;	//���[���h���W

							//���[���h�s�񂩂�X�P�[�����O�A��]�A�ړ������o��
	worldm.Decompose(scale, rotation, translation);

	//�X�P�[���Œ�
	scale = Vector3(25.0f, 25.0f, 25.0f);

	//�e�p�[�c���番���A�Ɨ�������
	m_Obj[PARTS_BULLET].SetObjParent(nullptr);
	m_Obj[PARTS_BULLET].SetScale(scale);
	m_Obj[PARTS_BULLET].SetRotQ(rotation);
	m_Obj[PARTS_BULLET].SetTrans(translation);
	m_Obj[PARTS_BULLET].SetDrawState(true);

	//�e�ۃp�[�c�̑��x�̐ݒ�
	m_BulletVel = Vector3(0, 0, -1);
	//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	m_FireFlag = true;

	//ADX2Le::Play(CRI_CUESHEET_0_LASER2);

	//m_shootCount++;
}

/// <summary>
/// �e�̔��ˁA�O�i�Ȃ�
/// </summary>
void Boss::BossBullet()
{
	//�e�ۃp�[�c�̔���
	//����
	if (m_startShootCount > m_startShootTime)
	{
		if (!m_FireFlag)
		{
			//����ł����甲����
			//if (GetDeathState()) return;
			FireBullet();
		}
		m_startShootCount = 0;
	}

	//�e�ۃp�[�c�̑O�i�����A��������
	if (m_FireFlag)
	{
		//���@�̃x�N�g��
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
	//���˒��ł͂Ȃ��ꍇ
	if (!m_FireFlag) return;

	//�ꏊ�����Z�b�g
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