/// <summary>
/// GS2 04 入山奨
/// 自作ライブラリヘッダー
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

	//int描画<unique_ptr>
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float描画<unique_ptr>
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//int描画<ptr>
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float描画<ptr>
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(DirectX::SpriteBatch& spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//int描画<shared_ptr>
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n);
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const RECT rect, int x, int y, int n, const DirectX::SimpleMath::Vector2 scale);

	//float描画<shared_ptr>
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point);
	void DrawNum(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numtexture, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pointtexture, const RECT rect, int x, int y, float n, int point, const DirectX::SimpleMath::Vector2 scale);

	//RECTをfloat4つで入力したらそれを返す
	RECT FloatConvertToRECT(const float top, const float left, const float bottom, const float right);

	//座標指定場所に指定したテクスチャを表示<unique_ptr>
	void DrawTexture(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos);
	void DrawTexture(const std::unique_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos, const DirectX::SimpleMath::Vector2 size);

	//座標指定場所に指定したテクスチャを表示<shared_ptr>
	void DrawTexture(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos);
	void DrawTexture(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2 pos, const DirectX::SimpleMath::Vector2 size);

	//1か-1をリターン
	int MinusOrPlus();

	//RECTからVector2でWidth,Heightを出す
	DirectX::SimpleMath::Vector2 RECTtoWidthHeight(const RECT rect);

	void LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, const wchar_t* fileName);

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		//static初期化
		static void InitializeStatic(const std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

		//テクスチャのロード(一括設定(RECTなし)
		void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos);

		//テクスチャのロード(一括設定)
		void LoadTextureSetAll(const wchar_t* fileName, const DirectX::SimpleMath::Vector2 imageSize, const DirectX::SimpleMath::Vector2 scale, const DirectX::SimpleMath::Vector2 pos, const RECT rect);

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

	//class Sprite
	//{
	//public:

	//private:
	//	RECT m_rect;
	//	DirectX::SimpleMath::Vector2 m_scale;
	//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//};

	/// <summary>
	/// 読み込みに使用
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