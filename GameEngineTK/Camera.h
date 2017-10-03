/// <summary>
/// �J�����𐧌䂷��N���X
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	//�������烁���o�֐�������
	Camera(int width,int height);
	virtual ~Camera();
	//�X�V
	virtual void Update();
	//�r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetView();
	//�ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& GetProjection();

	//���_���W�̃Z�b�g
	void SetEyePos(const DirectX::SimpleMath::Vector3 eyepos);
	//�����_�̃Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3 refpos);
	//�J�����̏�����x�N�g���̃Z�b�g
	void SetUpvec(const DirectX::SimpleMath::Vector3 upvec);
	//������������p�̃Z�b�g
	void SetFovY(const float fovY);
	//�A�X�y�N�g��̃Z�b�g
	void SetAspect(const float aspect);
	//�j�A�N���b�v�̃Z�b�g
	void SetNearClip(const float nearclip);
	//�t�@�[�N���b�v�̃Z�b�g
	void SetFarClip(const float farclip);

protected:
	//�J�����̈ʒu(���_���W)
	DirectX::SimpleMath::Vector3 m_eyepos;
	//�J�����̌��Ă����(�����_/�Q�Ɠ_/���ړ_)
	DirectX::SimpleMath::Vector3 m_refpos;
	//�J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;

	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	//������������p
	float m_fovY;
	//�A�X�y�N�g��(���c�̔䗦)
	float m_aspect;
	//�j�A�N���b�v(��O�̕\�����E)
	float m_nearclip;
	//�t�@�[�N���b�v(���̕\�����E)
	float m_farclip;

	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
};