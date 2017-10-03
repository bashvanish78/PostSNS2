#pragma once
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "MyFuncsXTK.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	//static初期化
	static void InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch,const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

	//テクスチャのロード(一括設定(RECTなし)
	void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos);

	//テクスチャのロード(一括設定)
	void LoadTextureSetAll(const wchar_t* fileName,const DirectX::SimpleMath::Vector2 imageSize,const DirectX::SimpleMath::Vector2 scale,const DirectX::SimpleMath::Vector2 pos, const RECT rect);

	//テクスチャのロード
	void LoadTexture(const wchar_t* fileName);
	//RECTのセット
	void SetRect(const RECT rect) { m_rect = rect; }
	//元画像のサイズセット
	void SetImageSize(const DirectX::SimpleMath::Vector2 imageSize) { m_imageSize = imageSize; }
	//倍率のセット
	void SetScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	//位置のセット
	void SetPos(const DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }

	//テクスチャのゲッター
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() { return m_texture; }
	//RECTのゲット
	RECT GetRect() { return m_rect; }
	//元画像のサイズゲット
	DirectX::SimpleMath::Vector2 GetImageSize() { return m_imageSize; }
	//倍率のゲット
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
	//位置のゲット
	DirectX::SimpleMath::Vector2 GetPos() { return m_pos; }

	//描画
	void Draw();
	//int数値描画
	void DrawNum(int num);
	//float数値描画
	void DrawNum(float num, int point, Sprite pointSprite);


private:
	//スプライトバッヂ
	static std::shared_ptr<DirectX::SpriteBatch> Sprite::m_spriteBatch;
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>  Sprite::m_d3dDevice;

	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//RECT
	RECT m_rect;
	//元画像のサイズ
	DirectX::SimpleMath::Vector2 m_imageSize;
	//拡大倍率
	DirectX::SimpleMath::Vector2 m_scale;
	//位置
	DirectX::SimpleMath::Vector2 m_pos;
};

