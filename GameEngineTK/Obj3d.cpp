
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ�
// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// �f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// �J����
Camera* Obj3d::m_camera;
// �ėp�X�e�[�g�ݒ�
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// �G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	//�ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//�G�t�F�N�g�t�@�N�g���쐬
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{

	//�����o�ϐ���������
	m_ObjParent = nullptr;
	m_scale = Vector3(1, 1, 1);
	//�f�t�H���g�ł̓I�C���[�p
	m_useQuaternion = false;
	//�f�t�H���g�ŕ\��
	m_drawState = true;
}

//Obj3d::Obj3d(const Obj3d& obj)
//{
//	this->m_model = std::move(obj.m_model);
//}

//Obj3d::Obj3d(Obj3d && obj)
//{
//	this->m_model = std::move(obj.m_model);
//
//	this->m_rotation = std::move(obj.m_rotation);
//}

void Obj3d::Finalize()
{
	m_model.reset();
}

//���f����ǂݍ���
void Obj3d::LoadModel(const wchar_t * filename)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), filename, *m_factory);
}

//���[���h�s��̌v�Z
void Obj3d::Update()
{
	Matrix scalemat = Matrix::CreateScale(m_scale);
	Matrix rotmat;
	if (m_useQuaternion)
		//�N�H�[�^�j�I���ŉ�]�̌v�Z
	{
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{
		//�I�C���[�p�ŉ�]���v�Z
		//Z����]
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		//X����]
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		//Y����]
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);

		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	//���s�ړ�
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	//���[���h�s�������
	m_world = scalemat * rotmat * transmat;

	if (m_ObjParent != NULL)
	{
		//�e������ꍇ�͐e�̊|���Z(�q�̍s��*�e�̍s��)
		m_world *= m_ObjParent->GetWorld();
	}


}

//�`��
void Obj3d::Draw()
{
	if (m_model != NULL && m_drawState)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states, 
			m_world,
			m_camera->GetView(), 
			m_camera->GetProjection());
	}	
}

