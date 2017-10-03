/// <summary>
/// GS2 04 ���R��
/// ���색�C�u�����w�b�_�[
/// </summary>
#pragma once

#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>
#include <vector>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include <WICTextureLoader.h>

namespace MyFuncsXTK
{

	//int�`��<unique_ptr>
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float�`��<unique_ptr>
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//int�`��<ptr>
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float�`��<ptr>
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//int�`��<shared_ptr>
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float�`��<shared_ptr>
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//RECT��float4�œ��͂����炻���Ԃ�
	RECT FloatConvertToRECT(const float top, const float left, const float bottom, const float right);

	//���W�w��ꏊ�Ɏw�肵���e�N�X�`����\��<unique_ptr>
	void DrawTexture(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos);
	void DrawTexture(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos, const DirectX::SimpleMath::Vector2 size);

	//���W�w��ꏊ�Ɏw�肵���e�N�X�`����\��<shared_ptr>
	void DrawTexture(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos);
	void DrawTexture(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos, const DirectX::SimpleMath::Vector2 size);

	//1��-1�����^�[��
	int MinusOrPlus();

	//RECT����Vector2��Width,Height���o��
	DirectX::SimpleMath::Vector2 RECTtoWidthHeight(const RECT rect);

	void LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, const wchar_t* fileName);

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		//static������
		static void InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

		//�e�N�X�`���̃��[�h(�ꊇ�ݒ�(RECT�Ȃ�)
		void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos);

		//�e�N�X�`���̃��[�h(�ꊇ�ݒ�)
		void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos, const RECT rect);

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

	//class Sprite
	//{
	//public:

	//private:
	//	RECT m_rect;
	//	DirectX::SimpleMath::Vector2 m_scale;
	//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//};

	/// <summary>
	/// �ǂݍ��݂Ɏg�p
	/// </summary>
	namespace MyDX
	{
		inline void ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				// Set a breakpoint on this line to catch DirectX API errors
				throw std::exception();
			}
		}
	}

}