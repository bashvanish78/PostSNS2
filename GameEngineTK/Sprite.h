#pragma once
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "MyFuncsXTK.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	//static������
	static void InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch,const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

	//�e�N�X�`���̃��[�h(�ꊇ�ݒ�(RECT�Ȃ�)
	void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos);

	//�e�N�X�`���̃��[�h(�ꊇ�ݒ�)
	void LoadTextureSetAll(const wchar_t* fileName,const DirectX::SimpleMath::Vector2 imageSize,const DirectX::SimpleMath::Vector2 scale,const DirectX::SimpleMath::Vector2 pos, const RECT rect);

	//�e�N�X�`���̃��[�h
	void LoadTexture(const wchar_t* fileName);
	//RECT�̃Z�b�g
	void SetRect(const RECT rect) { m_rect = rect; }
	//���摜�̃T�C�Y�Z�b�g
	void SetImageSize(const DirectX::SimpleMath::Vector2 imageSize) { m_imageSize = imageSize; }
	//�{���̃Z�b�g
	void SetScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	//�ʒu�̃Z�b�g
	void SetPos(const DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }

	//�e�N�X�`���̃Q�b�^�[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() { return m_texture; }
	//RECT�̃Q�b�g
	RECT GetRect() { return m_rect; }
	//���摜�̃T�C�Y�Q�b�g
	DirectX::SimpleMath::Vector2 GetImageSize() { return m_imageSize; }
	//�{���̃Q�b�g
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
	//�ʒu�̃Q�b�g
	DirectX::SimpleMath::Vector2 GetPos() { return m_pos; }

	//�`��
	void Draw();
	//int���l�`��
	void DrawNum(int num);
	//float���l�`��
	void DrawNum(float num, int point, Sprite pointSprite);


private:
	//�X�v���C�g�o�b�a
	static std::shared_ptr<DirectX::SpriteBatch> Sprite::m_spriteBatch;
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>  Sprite::m_d3dDevice;

	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//RECT
	RECT m_rect;
	//���摜�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_imageSize;
	//�g��{��
	DirectX::SimpleMath::Vector2 m_scale;
	//�ʒu
	DirectX::SimpleMath::Vector2 m_pos;
};

