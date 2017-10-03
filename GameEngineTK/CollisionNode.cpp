#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = false;

/// <summary>
/// CollisionNode�̊֐�
/// </summary>

void CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.SetObjParent(parent);
}

/// <summary>
/// SphereNode�̊֐�
/// </summary>

SphereNode::SphereNode()
{
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	//�f�o�b�O�\���p���f���̏���
	m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

void SphereNode::Update()
{
	m_Obj.SetTrans(m_trans);

	m_Obj.SetScale(Vector3(m_LocalRadius));

	m_Obj.Update();

	//���苅�̗v�f���v�Z
	{
		const Matrix worldm = m_Obj.GetWorld();

		//���f�����W�n�ł̒��S�_
		Vector3 center(0, 0, 0);
		//���f�����W�n�ł̉E�[�̓_
		Vector3 right(1, 0, 0);

		//���[���h���W�n�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		//���苅�̗v�f����
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);

	}
}

void SphereNode::Draw()
{
	m_Obj.Draw();
}


