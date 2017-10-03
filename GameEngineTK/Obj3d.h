/// <summary>
/// 3D�I�u�W�F�N�g�̃N���X
/// </summary>

#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	//�ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	//�ÓI�����o�ϐ�

	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

	//�J�����̃I�u�W�F�N�g
	static Camera* Obj3d::m_camera;

	//�ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

	//�G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

public:
	//�����o�֐�
	Obj3d();

	//�R�s�[�R���X�g���N�^
	//Obj3d(const Obj3d& obj);

	//���[�u�R���X�g���N�^
	//Obj3d(Obj3d&& obj);

	void Finalize();

	//CMO�t�@�C����ǂݍ���(���f���ǂݍ���)
	void LoadModel(const wchar_t* filename);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//SetMAU
	//�X�P�[�����O(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	//��]�p(XYZ/�I�C���[�p)
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; m_useQuaternion = false;}
	//��]�p(�N�H�[�^�j�I��)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rotation) { m_rotationQ = rotation; m_useQuaternion = true; }
	//���s�ړ�(XYZ)
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	//�e��Obj3d�ւ̃|�C���^
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	//���[���h�s��
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	//�\���X�e�[�g
	void SetDrawState(const bool drawState) { m_drawState = drawState; }


	//GetMAU
	//�X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	//��]�p(XYZ)
	const DirectX::SimpleMath::Vector3& GetRot() { return m_rotation; }
	//��]�p(�N�H�[�^�j�I��)
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_rotationQ; }
	//���s�ړ�(XYZ)
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_translation; }
	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	//�\�����Ă��邩
	const bool& GetDrawState() { return m_drawState; }
	//void Calc();

private:
	//�����o�ϐ�

	//���f��
	std::unique_ptr<DirectX::Model> m_model;
	//�X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//��]�p(XYZ/�I�C���[�p)
	DirectX::SimpleMath::Vector3 m_rotation;
	//��]�p(�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	//���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;
	//��]���N�H�[�^�j�I���Ŏg�p���Ă��邩
	bool m_useQuaternion;
	//�\�����邩
	bool m_drawState;
};

