#include <SimpleMath.h>
#include"Obj3d.h"
#include "CollisionNode.h"


#pragma once
class Mine
{
public:

	//���@�p�[�c
	enum PARTS
	{
		PARTS_BODY,	//�{��

		PARTS_NUM
	};

	//�R���X�g���N�^
	Mine();
	//�f�X�g���N�^
	~Mine();
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
	//��]��ݒ�
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//�X�P�[����ݒ�
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//�S�g�p�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�f�o�b�O��ԕύX
	void ChangeDebug();

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

	//�S�g�p�����蔻��I�u�W�F�N�g
	SphereNode m_CollisionNodeBody;

	//�f�o�b�O
	bool m_debug;
};

