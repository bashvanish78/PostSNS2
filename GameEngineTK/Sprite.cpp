#include "pch.h"
#include "Sprite.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace MyFuncsXTK;

using Microsoft::WRL::ComPtr;

//�ÓI�����o�ϐ�
//�X�v���C�g�o�b�a
std::shared_ptr<DirectX::SpriteBatch> Sprite::m_spriteBatch;
//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>  Sprite::m_d3dDevice;

Sprite::Sprite()
{
	m_rect.bottom = (long)0;
	m_rect.left = (long)0;
	m_rect.right = (long)0;
	m_rect.top = (long)0;
}

Sprite::~Sprite()
{
}

//static������
void Sprite::InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice)
{
	m_spriteBatch = spriteBatch;
	m_d3dDevice = d3dDevice;
}

void Sprite::LoadTextureSetAll(const wchar_t * fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos)
{
	//���\�[�X�����擾
	ComPtr<ID3D11Resource> resource;
	//�摜�ǂݍ���
	MyDX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), fileName,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));

	m_imageSize = imageSize;
	m_scale = scale;
	m_pos = pos;
}

void Sprite::LoadTextureSetAll(const wchar_t * fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos, const RECT rect)
{
	//���\�[�X�����擾
	ComPtr<ID3D11Resource> resource;
	//�摜�ǂݍ���
	MyDX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), fileName,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));
	
	m_rect = rect;
	m_imageSize = imageSize;
	m_scale = scale;
	m_pos = pos;
}

//�e�N�X�`���̃��[�h
void Sprite::LoadTexture(const wchar_t * fileName)
{
	//���\�[�X�����擾
	ComPtr<ID3D11Resource> resource;
	//�摜�ǂݍ���
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), fileName,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));
}

//�`��
void Sprite::Draw()
{
	CommonStates states(m_d3dDevice.Get());

	//�X�v���C�g�̕`��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	//RECT�Ȃ�
	if (m_rect.left == 0 && m_rect.bottom == 0 && m_rect.right == 0 && m_rect.top == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//�e�N�X�`��
			m_pos,		//�X�N���[�����W
			nullptr,				//Rect
			Colors::White,		//���̂܂܂̐F
			XMConvertToRadians(0),//��]
			Vector2(m_imageSize.x / 2.f, m_imageSize.y / 2.f));			//����
	}
	else
	{

		m_spriteBatch->Draw(
			m_texture.Get(),	//�e�N�X�`��
			m_pos,		//�X�N���[�����W
			&m_rect,				//Rect
			Colors::White,		//���̂܂܂̐F
			XMConvertToRadians(0),//��]
			Vector2(m_imageSize.x / 2.f, m_imageSize.y / 2.f));			//����
	}

	m_spriteBatch->End();
}

void Sprite::DrawNum(int num)
{
	int w = num;	//��Ɨp
	int i = 0;	//������

	CommonStates states(m_d3dDevice.Get());

	//�X�v���C�g�̕`��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	if (w == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//�e�N�X�`��
			SimpleMath::Vector2(m_pos.x * 1.f, m_pos.y * 1.f),		//�X�N���[�����W
			&m_rect,				//Rect
			Colors::White,		//���̂܂܂̐F
			XMConvertToRadians(0),//��]
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//����
			m_scale);	//�X�P�[��
	}
	else
	{
		while (w)
		{

			//�ꎞ�I��RECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//�e�N�X�`��
				SimpleMath::Vector2((m_pos.x - i * (m_rect.right - m_rect.left) * m_scale.x) + 16, m_pos.y * 1.f),		//�X�N���[�����W
				&rect2,				//Rect
				Colors::White,		//���̂܂܂̐F
				XMConvertToRadians(0),//��]
				Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),	//����
				m_scale);	//�X�P�[��

			w = w / 10;
			i++;
		}
	}

	m_spriteBatch->End();
}

void Sprite::DrawNum(float num, int point, Sprite pointSprite)
{
	CommonStates states(m_d3dDevice.Get());

	//�X�v���C�g�̕`��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	int w = (int)num;	//��Ɨp
	int i = 0;	//������

	int num2 = 1;

	for (int i = 1; i <= point; i++)
	{
		num2 *= 10;
	}

	int w2 = (int)((num - (int)num) * num2);

	if (w == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//�e�N�X�`��
			SimpleMath::Vector2(m_pos.x * 1.f, m_pos.y * 1.f),		//�X�N���[�����W
			&m_rect,				//Rect
			Colors::White,		//���̂܂܂̐F
			XMConvertToRadians(0),//��]
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//����
			m_scale);//�{��
	}
	else
	{
		while (w)
		{
			//�ꎞ�I��RECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//�e�N�X�`��
				SimpleMath::Vector2((m_pos.x - i * (m_rect.right - m_rect.left) * m_scale.x * 1.f) + 16, m_pos.y * 1.f),		//�X�N���[�����W
				&rect2,				//Rect
				Colors::White,		//���̂܂܂̐F
				XMConvertToRadians(0),//��]
				Vector2((m_rect.right - m_rect.left) * m_scale.x, (m_rect.bottom - m_rect.top) / 2.f),			//����
				m_scale);//�{��

			w = w / 10;
			i++;
		}
	}


	//�����_
	pointSprite.SetPos(Vector2(m_pos.x + (m_rect.right - m_rect.left) * 1.f, m_pos.y * 1.f));
	//pointSprite.SetRect(FloatConvertToRECT(0, 0, 0, 0));
	pointSprite.SetImageSize(Vector2((m_rect.right - m_rect.left), (m_rect.bottom - m_rect.top)));
	pointSprite.SetScale(m_scale);

	m_spriteBatch->Draw(
		pointSprite.GetTexture().Get(),	//�e�N�X�`��
		pointSprite.GetPos(),		//�X�N���[�����W
		nullptr,				//Rect
		Colors::White,		//���̂܂܂̐F
		XMConvertToRadians(0),//��]
		Vector2(pointSprite.GetImageSize().x / 2.f, pointSprite.GetImageSize().y / 2.f),//����
		pointSprite.GetScale());	//�{��

	i = 0;

	if (w2 == 0)
	{

		m_spriteBatch->Draw(
			m_texture.Get(),	//�e�N�X�`��
			SimpleMath::Vector2((m_pos.x + (m_rect.right - m_rect.left) * 2.f), m_pos.y * 1.f),		//�X�N���[�����W
			&m_rect,				//Rect
			Colors::White,		//���̂܂܂̐F
			XMConvertToRadians(0),//��]
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//����
			m_scale);	//�{��
	}
	else
	{
		while (w2)
		{

			//�ꎞ�I��RECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w2 % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w2 % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//�e�N�X�`��
				SimpleMath::Vector2((m_pos.x + (m_rect.right - m_rect.left) * 2.f - i * (m_rect.right - m_rect.left) * m_scale.x) + 16, m_pos.y * 1.f),		//�X�N���[�����W
				&rect2,				//Rect
				Colors::White,		//���̂܂܂̐F
				XMConvertToRadians(0),//��]
				Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),			//����
				m_scale);	//�{��

			w2 = w2 / 10;
			i++;
		}
	}

	m_spriteBatch->End();
}
