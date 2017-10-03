/// <summary>
/// 3Dオブジェクトのクラス
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
	//静的メンバ関数
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	//静的メンバ変数

	//デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

	//デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

	//カメラのオブジェクト
	static Camera* Obj3d::m_camera;

	//汎用ステート設定
	static std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

	//エフェクトファクトリ
	static std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

public:
	//メンバ関数
	Obj3d();

	//コピーコンストラクタ
	//Obj3d(const Obj3d& obj);

	//ムーブコンストラクタ
	//Obj3d(Obj3d&& obj);

	void Finalize();

	//CMOファイルを読み込み(モデル読み込み)
	void LoadModel(const wchar_t* filename);
	//更新
	void Update();
	//描画
	void Draw();

	//SetMAU
	//スケーリング(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	//回転角(XYZ/オイラー角)
	void SetRot(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; m_useQuaternion = false;}
	//回転角(クォータニオン)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rotation) { m_rotationQ = rotation; m_useQuaternion = true; }
	//平行移動(XYZ)
	void SetTrans(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	//親のObj3dへのポインタ
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	//ワールド行列
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	//表示ステート
	void SetDrawState(const bool drawState) { m_drawState = drawState; }


	//GetMAU
	//スケーリング(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	//回転角(XYZ)
	const DirectX::SimpleMath::Vector3& GetRot() { return m_rotation; }
	//回転角(クォータニオン)
	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_rotationQ; }
	//平行移動(XYZ)
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_translation; }
	//ワールド行列
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	//表示しているか
	const bool& GetDrawState() { return m_drawState; }
	//void Calc();

private:
	//メンバ変数

	//モデル
	std::unique_ptr<DirectX::Model> m_model;
	//スケーリング(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//回転角(XYZ/オイラー角)
	DirectX::SimpleMath::Vector3 m_rotation;
	//回転角(クォータニオン)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	//平行移動(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//親のObj3dへのポインタ
	Obj3d* m_ObjParent;
	//回転をクォータニオンで使用しているか
	bool m_useQuaternion;
	//表示するか
	bool m_drawState;
};

