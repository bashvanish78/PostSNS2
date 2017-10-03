#include <SimpleMath.h>
#include "Obj3d.h"
#include "CollisionNode.h"
#include <Keyboard.h>

#pragma once
class Player
{
public:
	//�d�͉����x
	const float GRAVITY_ACC = 0.03f;
	//�W�����v����
	const float JUMP_SPEED_FIRST = 0.5f;
	//�������x����
	const float JUMP_SPEED_MAX = 0.3f;

	//���@�p�[�c
	enum PARTS
	{
		PARTS_FIGHTER,//�@��
		PARTS_R_WING,//�E��
		PARTS_L_WING,//����
		PARTS_BULLET,//�e
		PARTS_BOOST,//�u�[�X�g

		PARTS_NUM
	};

	Player(DirectX::Keyboard* key);
	void Initialize();
	~Player();
	void Update();
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
	//���x�擾
	const DirectX::SimpleMath::Vector3& GetVelocity();
	//�e�𔭎˂��Ă����Ԃ��擾
	const bool GetFireFlag();


	//���W��ݒ�
	void SetTrans(DirectX::SimpleMath::Vector3& trans);
	//��]��ݒ�
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//�X�P�[����ݒ�
	void SetScale(DirectX::SimpleMath::Vector3& scale);

	//�e�ۂ𔭎�
	void FireBullet();
	//�e�ۂ�߂�
	void ResetBullet();

	//�e�ۗp�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	//�S�g�p�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//���񂾂Ƃ�
	void SetDeathState(const bool& deathstate) { m_deathstate = deathstate; }

	const bool& GetDeathState() { return m_deathstate; }

	//���S�񐔂̃J�E���g
	void PlusDeathCount() { m_defeatCount++; }

	//���X�|�[���J�E���g������
	void ResetRespawnCount() { m_respawnCount = 0; }

	//�f�o�b�O��ԕύX
	void ChangeDebug();

	void StartJump();

	void StartFall();

	void StopJump();

	//�H�̏��(0,����.1,����.2,�E��)
	void SetWingState(const int& wingstate) { m_wingState = wingstate; }
	const int& GetWingState() { return m_wingState; }

	//���G���
	void SetInvincibleState(const bool& invinciblestate) { m_invincibleState = invinciblestate; }
	const bool& GetInvincibleState() { return m_invincibleState; }

	const int& GetInvincibleCount() { return m_invincibleCount; }

	void Damage();

	void SetGameSceneResume(const bool resume) { m_gamesceneResume = resume; }

	int GetShootTimeCount() { return m_shootTimeCount; }
	int GetDamageCount() { return m_damageCount; }
	int GetDefeatCount() { return m_defeatCount; }

	void PlayerDataReset();

private:
	//�p�[�c�M�~�b�N
	void PartsGimic();
	void PlayerMove(DirectX::Keyboard::State kb);
	void PlayerBullet();

	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//�e�𔭎˂��Ă���̃J�E���g
	int m_shootcount;
	//�e�������鎞��
	const int m_deletecount = 30;

	//�e�̊p�x
	float bulletangle;
	float cannonangle;

	//���˂��Ă�����
	bool m_FireFlag;

	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//�e�ۗp�����蔻��I�u�W�F�N�g
	SphereNode m_CollisionNodeBullet;
	//�S�g�p�����蔻��I�u�W�F�N�g
	SphereNode m_CollisionNodeBody;
	
	//���x
	DirectX::SimpleMath::Vector3 m_Velocity;
	//�������t���O
	bool m_isJump;

	//�f�o�b�O
	bool m_debug;

	//�u�[�X�g�̃��f���g�k
	int m_boostModelCount;
	const int m_boostModelTime = 6;
	bool m_boostModelState;

	bool m_deathstate;

	const int m_respawnTime = 90;
	int m_respawnCount;

	int m_wingState;

	const int m_invincibleTime = 60;
	int m_invincibleCount;

	bool m_invincibleState;

	bool m_gamesceneResume;

	int m_shootTimeCount;
	int m_defeatCount;
	int m_damageCount;
};

