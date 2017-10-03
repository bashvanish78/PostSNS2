#include "pch.h"
#include "Sprite.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace MyFuncsXTK;

using Microsoft::WRL::ComPtr;

//静的メンバ変数
//スプライトバッヂ
std::shared_ptr<DirectX::SpriteBatch> Sprite::m_spriteBatch;
//デバイス
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

//static初期化
void Sprite::InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice)
{
	m_spriteBatch = spriteBatch;
	m_d3dDevice = d3dDevice;
}

void Sprite::LoadTextureSetAll(const wchar_t * fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos)
{
	//リソース情報を取得
	ComPtr<ID3D11Resource> resource;
	//画像読み込み
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
	//リソース情報を取得
	ComPtr<ID3D11Resource> resource;
	//画像読み込み
	MyDX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), fileName,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));
	
	m_rect = rect;
	m_imageSize = imageSize;
	m_scale = scale;
	m_pos = pos;
}

//テクスチャのロード
void Sprite::LoadTexture(const wchar_t * fileName)
{
	//リソース情報を取得
	ComPtr<ID3D11Resource> resource;
	//画像読み込み
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), fileName,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));
}

//描画
void Sprite::Draw()
{
	CommonStates states(m_d3dDevice.Get());

	//スプライトの描画
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	//RECTなし
	if (m_rect.left == 0 && m_rect.bottom == 0 && m_rect.right == 0 && m_rect.top == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//テクスチャ
			m_pos,		//スクリーン座標
			nullptr,				//Rect
			Colors::White,		//そのままの色
			XMConvertToRadians(0),//回転
			Vector2(m_imageSize.x / 2.f, m_imageSize.y / 2.f));			//中央
	}
	else
	{

		m_spriteBatch->Draw(
			m_texture.Get(),	//テクスチャ
			m_pos,		//スクリーン座標
			&m_rect,				//Rect
			Colors::White,		//そのままの色
			XMConvertToRadians(0),//回転
			Vector2(m_imageSize.x / 2.f, m_imageSize.y / 2.f));			//中央
	}

	m_spriteBatch->End();
}

void Sprite::DrawNum(int num)
{
	int w = num;	//作業用
	int i = 0;	//文字数

	CommonStates states(m_d3dDevice.Get());

	//スプライトの描画
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	if (w == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//テクスチャ
			SimpleMath::Vector2(m_pos.x * 1.f, m_pos.y * 1.f),		//スクリーン座標
			&m_rect,				//Rect
			Colors::White,		//そのままの色
			XMConvertToRadians(0),//回転
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//中央
			m_scale);	//スケール
	}
	else
	{
		while (w)
		{

			//一時的なRECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//テクスチャ
				SimpleMath::Vector2((m_pos.x - i * (m_rect.right - m_rect.left) * m_scale.x) + 16, m_pos.y * 1.f),		//スクリーン座標
				&rect2,				//Rect
				Colors::White,		//そのままの色
				XMConvertToRadians(0),//回転
				Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),	//中央
				m_scale);	//スケール

			w = w / 10;
			i++;
		}
	}

	m_spriteBatch->End();
}

void Sprite::DrawNum(float num, int point, Sprite pointSprite)
{
	CommonStates states(m_d3dDevice.Get());

	//スプライトの描画
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	int w = (int)num;	//作業用
	int i = 0;	//文字数

	int num2 = 1;

	for (int i = 1; i <= point; i++)
	{
		num2 *= 10;
	}

	int w2 = (int)((num - (int)num) * num2);

	if (w == 0)
	{
		m_spriteBatch->Draw(
			m_texture.Get(),	//テクスチャ
			SimpleMath::Vector2(m_pos.x * 1.f, m_pos.y * 1.f),		//スクリーン座標
			&m_rect,				//Rect
			Colors::White,		//そのままの色
			XMConvertToRadians(0),//回転
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//中央
			m_scale);//倍率
	}
	else
	{
		while (w)
		{
			//一時的なRECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//テクスチャ
				SimpleMath::Vector2((m_pos.x - i * (m_rect.right - m_rect.left) * m_scale.x * 1.f) + 16, m_pos.y * 1.f),		//スクリーン座標
				&rect2,				//Rect
				Colors::White,		//そのままの色
				XMConvertToRadians(0),//回転
				Vector2((m_rect.right - m_rect.left) * m_scale.x, (m_rect.bottom - m_rect.top) / 2.f),			//中央
				m_scale);//倍率

			w = w / 10;
			i++;
		}
	}


	//小数点
	pointSprite.SetPos(Vector2(m_pos.x + (m_rect.right - m_rect.left) * 1.f, m_pos.y * 1.f));
	//pointSprite.SetRect(FloatConvertToRECT(0, 0, 0, 0));
	pointSprite.SetImageSize(Vector2((m_rect.right - m_rect.left), (m_rect.bottom - m_rect.top)));
	pointSprite.SetScale(m_scale);

	m_spriteBatch->Draw(
		pointSprite.GetTexture().Get(),	//テクスチャ
		pointSprite.GetPos(),		//スクリーン座標
		nullptr,				//Rect
		Colors::White,		//そのままの色
		XMConvertToRadians(0),//回転
		Vector2(pointSprite.GetImageSize().x / 2.f, pointSprite.GetImageSize().y / 2.f),//中央
		pointSprite.GetScale());	//倍率

	i = 0;

	if (w2 == 0)
	{

		m_spriteBatch->Draw(
			m_texture.Get(),	//テクスチャ
			SimpleMath::Vector2((m_pos.x + (m_rect.right - m_rect.left) * 2.f), m_pos.y * 1.f),		//スクリーン座標
			&m_rect,				//Rect
			Colors::White,		//そのままの色
			XMConvertToRadians(0),//回転
			Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),//中央
			m_scale);	//倍率
	}
	else
	{
		while (w2)
		{

			//一時的なRECT
			RECT rect2 = FloatConvertToRECT(0.f, (float)((w2 % 10)*(m_rect.right - m_rect.left)), (float)(m_rect.bottom - m_rect.top), (float)((w2 % 10)*(m_rect.right - m_rect.left) + (m_rect.right - m_rect.left)));

			m_spriteBatch->Draw(
				m_texture.Get(),	//テクスチャ
				SimpleMath::Vector2((m_pos.x + (m_rect.right - m_rect.left) * 2.f - i * (m_rect.right - m_rect.left) * m_scale.x) + 16, m_pos.y * 1.f),		//スクリーン座標
				&rect2,				//Rect
				Colors::White,		//そのままの色
				XMConvertToRadians(0),//回転
				Vector2((m_rect.right - m_rect.left) / 2.f, (m_rect.bottom - m_rect.top) / 2.f),			//中央
				m_scale);	//倍率

			w2 = w2 / 10;
			i++;
		}
	}

	m_spriteBatch->End();
}
