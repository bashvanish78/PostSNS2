#include "pch.h"
#include "Player.h"
#include "ADX2Le.h"
#include "Resources\Music\CueSheet_0.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* key)
{
	m_keyboard = key;
	m_FireFlag = false;

	m_debug = false;
}

void Player::Initialize()
{
	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_FIGHTER].LoadModel(L"Resources/Fighter.cmo");
	m_Obj[PARTS_R_WING].LoadModel(L"Resources/R_WING.cmo");
	m_Obj[PARTS_L_WING].LoadModel(L"Resources/L_WING.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/PlayerBullet.cmo");
	m_Obj[PARTS_BOOST].LoadModel(L"Resources/Boost.cmo");

	//�e�q�֌W�̍\�z(�q�p�[���ɐe��ݒ�)
	m_Obj[PARTS_R_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_L_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BOOST].SetObjParent(&m_Obj[PARTS_FIGHTER]);

	//�X�P�[���̎w��
	m_Obj[PARTS_FIGHTER].SetScale(Vector3(1.4f));

	//���W�̂���w��
	m_Obj[PARTS_FIGHTER].SetTrans(Vector3(0, 1, 0));
	m_Obj[PARTS_BOOST].SetTrans(Vector3(0, 0, 0.7f));

	//�e�͔��˂��ĂȂ��Ƃ���\��
	m_Obj[PARTS_BULLET].SetDrawState(false);
	//�������Ă��Ȃ��Ƃ���\��
	m_Obj[PARTS_BOOST].SetDrawState(false);

	m_cycle = 0.0f;

	//���@�e�̃J�E���g
	m_shootcount = 0;
	//�u�[�X�g�̃J�E���g�A�X�e�[�g
	m_boostModelCount = 0;
	m_boostModelState = false;

	//�e�ۗp�̂����蔻��m�[�h�̐ݒ������
	{
		m_CollisionNodeBullet.Initialize();
		//�e�I�u�W�F�N�g��ݒ�
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BULLET]);

		m_CollisionNodeBullet.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
	//�S�g�p�̂����蔻��m�[�h�̐ݒ������
	{
		m_CollisionNodeBody.Initialize();
		//�e�I�u�W�F�N�g��ݒ�
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_FIGHTER]);

		m_CollisionNodeBody.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBody.SetLocalRadius(0.3f);
	}

	m_isJump = false;

	m_deathstate = false;

	m_wingState = 0;

	m_respawnCount = 0;

	m_invincibleCount = 0;

	m_invincibleState = false;

	//ACF�t�@�C���ӂ��݂���
	ADX2Le::Initialize("Resources/Music/Shooting3D.acf");
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb", "Resources/Music/CueSheet_0.awb");

	m_shootTimeCount = 0;
	m_defeatCount = 0;
	m_damageCount = 0;
}


Player::~Player()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Finalize();
	}
}

void Player::Update()
{
	//�L�[�{�[�h�̏�Ԏ擾
	Keyboard::State kb = m_keyboard->GetState();

	m_keyboardTracker.Update(kb);

	PartsGimic();
	
	PlayerMove(kb);

	//����ł��玞�Ԍo�߂ŕ���
	if (GetDeathState())
	{
		m_respawnCount++;
		
		if (m_respawnTime < m_respawnCount)
		{
			m_respawnCount = 0;
			SetDeathState(false);
			SetWingState(0);
			m_Obj[PARTS_L_WING].SetDrawState(true);
			m_Obj[PARTS_R_WING].SetDrawState(true);
			SetInvincibleState(false);
			SetTrans(Vector3(0, 3, 0));
			SetRot(Vector3(0, 0, 0));
		}
	}

	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	PlayerBullet();

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

	switch (GetWingState())
	{
	case 0:
		m_Obj[PARTS_L_WING].SetDrawState(true);
		m_Obj[PARTS_R_WING].SetDrawState(true);
		break;
	case 1:
		m_Obj[PARTS_L_WING].SetDrawState(false);
		break;
	case 2:
		m_Obj[PARTS_R_WING].SetDrawState(false);
		break;
	}

	Calc();
}

void Player::Draw()
{
	//����ł����甲����
	if (GetDeathState()) return;

	//���G�J�E���g�������������甲����
	if (GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0) return;

	//�v���C���[�̕`��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	if(m_debug) m_CollisionNodeBullet.Draw();
	if (m_debug)m_CollisionNodeBody.Draw();
}

void Player::Calc()
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

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	return m_Obj[0].GetTrans();
}

const DirectX::SimpleMath::Matrix& Player::GetWorld()
{
	return m_Obj[0].GetWorld();
}

const DirectX::SimpleMath::Vector3 & Player::GetVelocity()
{
	return m_Velocity;
}

const bool Player::GetFireFlag()
{
	return m_FireFlag;
}

const DirectX::SimpleMath::Vector3& Player::GetScale()
{
	return m_Obj[0].GetScale();
}

const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	return m_Obj[0].GetRot();
}

void Player::SetScale(DirectX::SimpleMath::Vector3& scale)
{
	m_Obj[0].SetScale(scale);
}

void Player::SetRot(DirectX::SimpleMath::Vector3& rot)
{
	m_Obj[0].SetRot(rot);
}

void Player::SetTrans(DirectX::SimpleMath::Vector3& trans)
{
	m_Obj[0].SetTrans(trans);
}


void Player::FireBullet()
{
	//���˒��̏ꍇ
	if (m_FireFlag) return;

	//���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm = m_Obj[PARTS_BULLET].GetWorld();
	//���o���������i�[
	Vector3 scale;			//���[���h�X�P�[�����O
	Quaternion rotation;	//���[���h��]
	Vector3 translation;	//���[���h���W

	//���[���h�s�񂩂�X�P�[�����O�A��]�A�ړ������o��
	worldm.Decompose(scale, rotation, translation);

	//�X�P�[���Œ�
	scale = Vector3(1.0f, 1.0f, 1.0f);

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

	ADX2Le::PlaySe(CRI_CUESHEET_0_LASER2);

	m_shootTimeCount++;
}

void Player::ResetBullet()
{
	//���˒��ł͂Ȃ��ꍇ
	if (!m_FireFlag) return;
	
	//�ꏊ�����Z�b�g
	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BULLET].SetTrans(Vector3(0.0f, 0.0f, 0.0f));
	m_Obj[PARTS_BULLET].SetRot(Vector3(0,0,0));
	m_Obj[PARTS_BULLET].SetDrawState(false);

	m_FireFlag = false;

}

void Player::ChangeDebug()
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

/// <summary>
/// �W�����v���J�n
/// </summary>
void Player::StartJump()
{
	if (!m_isJump)
	{
		//������̏�����ݒ�
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

/// <summary>
/// �������J�n
/// </summary>
void Player::StartFall()
{
	if (!m_isJump)
	{
		//������̏�����ݒ�
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

/// <summary>
/// �����I��
/// </summary>
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

/// <summary>
/// �v���C���[���_���[�W���󂯂ĉH������
/// </summary>
void Player::Damage()
{
	//�ǂ��炩������(1,����.2,�E��)
	int wing = rand() % 2 + 1;
	SetWingState(wing);

	SetInvincibleState(true);
	m_damageCount++;
}

void Player::PlayerDataReset()
{
	m_shootTimeCount = 0;
	m_damageCount = 0;
	m_defeatCount = 0;
}

/// <summary>
/// �p�[�c�M�~�b�N
/// </summary>
void Player::PartsGimic()
{
	//�u�[�X�g���Ă�����
	if (m_Obj[PARTS_BOOST].GetDrawState())
	{

		m_boostModelCount++;

		//���Ԍo�߂ŃX�e�[�g�ύX
		if (m_boostModelCount > m_boostModelTime)
		{
			if (m_boostModelState)
			{
				m_boostModelState = false;
			}
			else
			{
				m_boostModelState = true;
			}
			m_boostModelCount = 0;
		}

		//�g�k���J��Ԃ�
		if (m_boostModelState)
		{
			m_Obj[PARTS_BOOST].SetScale(m_Obj[PARTS_BOOST].GetScale() + Vector3(0.05f, 0, 0.05f));
		}
		else
		{
			m_Obj[PARTS_BOOST].SetScale(m_Obj[PARTS_BOOST].GetScale() - Vector3(0.05f, 0, 0.05f));
		}
	}
}

void Player::PlayerMove(DirectX::Keyboard::State kb)
{
	//����ł����甲����
	if (GetDeathState()) return;

	//�펞�ړ�
	{
		//���@�̃x�N�g��
		Vector3 moveV(0.0f, 0.0f, -0.05f);

		//���V�t�g�L�[��������Ă��������
		if (kb.LeftShift)
		{
			moveV.z -= 0.2f;
			m_Obj[PARTS_BOOST].SetDrawState(true);
		}
		else
		{
			m_Obj[PARTS_BOOST].SetDrawState(false);
		}

		float yangle = m_Obj[0].GetRot().y;
		float xangle = m_Obj[0].GetRot().x;

		//�ړ��x�N�g�������@�̊p�x����]
		Matrix yrotmat = Matrix::CreateRotationY(yangle);
		Matrix xrotmat = Matrix::CreateRotationX(xangle);

		Matrix rotmat = xrotmat * yrotmat;
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);

		//���͂���Ă��Ȃ��ꍇ�܂�����
		m_Obj[0].SetRot(Vector3(xangle, yangle, 0));
	}
	//a�L�[�������Ă���ԍ�����
	if (kb.A)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRot().y;

		float xangle = m_Obj[0].GetRot().x;

		if (GetWingState() != 1)
		{
			m_Obj[0].SetRot(Vector3(xangle, angle + 0.03f, 0.5f));
		}
		else
		{
			m_Obj[0].SetRot(Vector3(xangle, angle + 0.01f, 0.5f));
		}
	}
	//d�L�[�������Ă���ԉE����
	if (kb.D)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRot().y;

		float xangle = m_Obj[0].GetRot().x;

		if (GetWingState() != 2)
		{
			m_Obj[0].SetRot(Vector3(xangle, angle - 0.03f, -0.5f));
		}
		else
		{
			m_Obj[0].SetRot(Vector3(xangle, angle - 0.01f, -0.5f));
		}
		//m_Obj[0].SetRot(Vector3(xangle, angle - 0.03f, -0.5f));
	}

	//w�L�[�������Ă���ԑO�ɌX����
	if (kb.W)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRot().x;

		float yangle = m_Obj[0].GetRot().y;

		float zangle = m_Obj[0].GetRot().z;

		if (angle > -1.4f) m_Obj[0].SetRot(Vector3(angle - 0.03f, yangle, zangle));

	}
	//s�L�[�������Ă���Ԍ��ɌX����
	if (kb.S)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRot().x;

		float yangle = m_Obj[0].GetRot().y;

		float zangle = m_Obj[0].GetRot().z;

		if(angle < 1.4f) m_Obj[0].SetRot(Vector3(angle + 0.03f, yangle, zangle));

	}
}

/// <summary>
/// �e�̔��ˁA�O�i�Ȃ�
/// </summary>
void Player::PlayerBullet()
{
	//�e�ۃp�[�c�̔���
	//����
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_gamesceneResume == false)
	{
		if (!m_FireFlag)
		{
			//����ł����甲����
			if (GetDeathState()) return;
			FireBullet();
		}
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
