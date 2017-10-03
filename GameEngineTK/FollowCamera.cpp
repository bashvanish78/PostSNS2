#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3(0.0f, 0.0f, 0.0f);
	m_targetAngle = Vector3(0.0f,0.0f,0.0f);
	m_keyboard = nullptr;
	m_Player = nullptr;
	m_fpsFlag = false;
}

void FollowCamera::Update()
{
	//�J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;

	//�L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	//�J�����̃t���O
	//static bool m_fpsFlag = true;

	//if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	//{
	//	//TPS,FPS�ؑ�
	//	//�t���O�؂�ւ�
	//	m_fpsFlag = !m_fpsFlag;
	//}

	//�v���C���[�̍��W�A�p�x���Z�b�g
	if (m_Player)
	{
		this->SetTargetPos(m_Player->GetTrans());
		
		this->SetTargetAngle(m_Player->GetRot());
	}


	//FPS�̃J��������
	if(m_fpsFlag == true)
	{

		Vector3 position;
		//�^�[�Q�b�g�̍��W�͎��@�̍��W�ɒǏ]
		position = m_targetPos + Vector3(0.0f, 0.2f, 0.0f);
		//�^�[�Q�b�g���W����J�������W�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);

		//���@�̌��ɉ�荞�ނ��߂̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle.y);
		//�J�����ւ̃x�N�g�����J�X
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		//�J�������W���v�Z
		//������Ɛi�񂾈ʒu���������W
		refpos = position + cameraV * 1.1f;
		//������i�񂾈ʒu���Q�Ɠ_���W
		eyepos = position + cameraV;

	}
	else
	//TPS�̃J��������
	{
		//�^�[�Q�b�g�̍��W�͎��@�̍��W�ɒǏ]
		refpos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
		//�^�[�Q�b�g���W����J�������W�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
		//���񂾏ꍇ�J��������������
		if (m_Player->GetDeathState())
		{
			cameraV.z = 20;
		}
		//���@�̌��ɉ�荞�ނ��߂̉�]�s��
		Matrix rotmaty = Matrix::CreateRotationY(m_targetAngle.y);
		Matrix rotmatx = Matrix::CreateRotationX(m_targetAngle.x);

		Matrix rotmat = rotmatx * rotmaty;
		//�J�����ւ̃x�N�g�����J�X
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		//�J�������W���v�Z
		eyepos = refpos + cameraV;

		//���_�����݈ʒu�����Ԃ��鏈��
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05;
		//�Q�Ɠ_�����݈ʒu�����Ԃ��鏈��
		refpos = m_refpos + (refpos - m_refpos) * 0.05;

	}



	//�����Z�b�g
	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	//Camera��Update���Ăяo��(���N���X�̍X�V�j
	Camera::Update();

}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(const DirectX::SimpleMath::Vector3 targetAngle)
{
	m_targetAngle = targetAngle;
}

void FollowCamera::SetKeyBoard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

void FollowCamera::SetPlayer(Player * player)
{
	m_Player = player;
}
