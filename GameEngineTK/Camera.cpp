#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�R���X�g���N�^
//����width(int),height(int)
Camera::Camera(int width, int height)
{
	//�����o�ϐ��̏�����
	//�J�������_
	m_eyepos = Vector3(0.0f, 0.0f, 5.0f);
	//�J�����Q�Ɠ_
	m_refpos = Vector3(0.0f, 0.0f, 0.0f);
	//������x�N�g��
	m_upvec = Vector3(0, 1, 0);

	//m_upvec.Normalize();

	//������������p
	m_fovY = XMConvertToRadians(60.0f);
	//�A�X�y�N�g��(���c�̔䗦)
	m_aspect = (float)width / height;
	//�j�A�N���b�v(��O�̕\�����E)
	m_nearclip = 0.1f;
	//�t�@�[�N���b�v(���̕\�����E)
	m_farclip = 1000.0f;

	//�r���[�s��
	m_view = Matrix::CreateLookAt(
		m_eyepos,	//�J�������_
		m_refpos,			//�J�����Q�Ɠ_
		m_upvec);		//������x�N�g��

	//�ˉe�s�񐶐�
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,			//����p(�㉺����)
		m_aspect,	//�A�X�y�N�g��
		m_nearclip,	//�j�A�N���b�v
		m_farclip);	//�t�@�[�N���b�v

}

//�f�X�g���N�^
Camera::~Camera()
{
}

//�X�V
void Camera::Update()
{
	//�r���[�s��
	m_view = Matrix::CreateLookAt(
		m_eyepos,	//�J�������_
		m_refpos,			//�J�����Q�Ɠ_
		m_upvec);		//������x�N�g��

	//�ˉe�s�񐶐�
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,			//����p(�㉺����)
		m_aspect,	//�A�X�y�N�g��
		m_nearclip,	//�j�A�N���b�v
		m_farclip);	//�t�@�[�N���b�v

}

const Matrix& Camera::GetView()
{
	//�r���[�s������^�[��
	return m_view;
}

const Matrix& Camera::GetProjection()
{
	//�ˉe�s������^�[��
	return m_proj;
}

//���_���W�̃����o�ϐ���eyepos���
void Camera::SetEyePos(const Vector3 eyepos)
{	
	m_eyepos = eyepos;
}

//�����_�̃����o�ϐ���refpos���
void Camera::SetRefPos(const Vector3 refpos)
{
	m_refpos = refpos;
}

//�J�����̏�����x�N�g���̃����o�ϐ���upvec���
void Camera::SetUpvec(const Vector3 upvec)
{
	m_upvec = upvec;
}

//������������p�̃����o�ϐ���fovY���
void Camera::SetFovY(const float fovY)
{
	m_fovY = fovY;
}

//�A�X�y�N�g��̃����o�ϐ���aspect���
void Camera::SetAspect(const float aspect)
{
	m_aspect = aspect;
}

//�j�A�N���b�v�̃����o�ϐ���nearclip���
void Camera::SetNearClip(const float nearclip)
{
	m_nearclip = nearclip;
}

//�t�@�[�N���b�v�̃����o�ϐ���farclip���
void Camera::SetFarClip(const float farclip)
{
	m_farclip = farclip;
}


