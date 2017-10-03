/// <summary>
/// ���@�ɒǏ]����J�����̃N���X
/// </summary>

#pragma once

#include <windows.h>
#include <Keyboard.h>
#include "Camera.h"
#include "Player.h"

class FollowCamera : public Camera
{
public:
	//���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
	FollowCamera(int width,int height);
	//�X�V
	void Update() override;
	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos);
	//�Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(const DirectX::SimpleMath::Vector3 targetAngle);
	//�L�[�{�[�h���Z�b�g
	void SetKeyBoard(DirectX::Keyboard* keyboard);
	//�v���C���[���Z�b�g
	void SetPlayer(Player* player);

protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̉�]�p
	DirectX::SimpleMath::Vector3 m_targetAngle;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//FPS�t���O
	bool m_fpsFlag;
	//�v���C���[�̃|�C���^
	Player* m_Player;
};

