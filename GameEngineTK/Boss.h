#include <SimpleMath.h>
#include"Obj3d.h"
#include "CollisionNode.h"

#pragma once
class Boss
{
public:

	//���@�p�[�c
	enum PARTS
	{
		PARTS_HEAD2,	//��2
		PARTS_BULLET,

		PARTS_NUM
	};

	//�R���X�g���N�^
	Boss();
	//�f�X�g���N�^
	~Boss();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�v�Z
	void Calc();

	//���W���K��
	const DirectX::SimpleMath::Vector3& GetTrans();
	//��]���擾
	const DirectX::SimpleMath::Vector3& GetRot();
	//�X�P�[�����擾
	const DirectX::SimpleMath::Vector3& GetScale();
	//���[���h���W���擾
	const DirectX::SimpleMath::Matrix& GetWorld();

	//���W��ݒ�
	void SetTrans(DirectX::SimpleMath::Vector3& trans);
	void Damage();
	//��]��ݒ�
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//�X�P�[����ݒ�
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//�e�ۗp�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	//�S�g�p�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�f�o�b�O��ԕύX
	void ChangeDebug();

	const int& GetHP() { return m_HP; }
	void SetHP(const int& HP) { m_HP = HP; }

	const int& GetDefaultHP() { return m_defaultHP; }

	//���G���
	void SetInvincibleState(const bool& invinciblestate) { m_invincibleState = invinciblestate; }
	const bool& GetInvincibleState() { return m_invincibleState; }

	const int& GetInvincibleCount() { return m_invincibleCount; }

	//�e�𔭎˂��Ă����Ԃ��擾
	const bool GetFireFlag();

private:
	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//���˂��Ă��������܂�
	int shootcount;
	//�����鎞��
	int deletecount;
	//�e�̊p�x
	float bulletangle;
	float cannonangle;

	float m_DistAngle;
	int m_timer;

	//�e�ۗp�����蔻��I�u�W�F�N�g
	SphereNode m_CollisionNodeBullet;
	//�S�g�p�����蔻��I�u�W�F�N�g
	SphereNode m_CollisionNodeBody;

	//�f�o�b�O
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
	//���˂��Ă�����
	bool m_FireFlag;

	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	void BossBullet();
	//�e�ۂ�߂�
	void ResetBullet();

	int m_shootcount;
	//�e�������鎞��
	const int m_deletecount = 30;

	int m_startShootCount;
	const int m_startShootTime = 60;

};
