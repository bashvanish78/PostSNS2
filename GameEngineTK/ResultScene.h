#pragma once

#include "MyFuncsXTK.h"
#include <SimpleMath.h>

#include "TitleScene.h"
#include "StepTimer.h"
#include "SceneState.h"
#include "Obj3d.h"
//#include "DebugCamera.h"
#include "Camera.h"

//#include "Sprite.h"

class SceneManager;
class SceneState;

class ResultScene : public SceneState
{
private:
	static SceneState* pState;

public:
	ResultScene();
	~ResultScene();

	static SceneState* GetInstance();

	void Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneRender();

	static void Dispose();

	void SetPlayerDataOnResult(Player* player);

	/// <summary>
	/// Result
	/// </summary>
public:

	//Game();

	// Initialization and management
	void Initialize(HWND& window, int& width, int& height, std::shared_ptr<DirectX::Keyboard> keyboard);

	// Basic game loop
	void Tick(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer, SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	////変数追加が可能

	bool m_gameHistory;
	bool m_chooseGame;

	//スプライトバッヂ
	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//スプライト
	MyFuncsXTK::Sprite m_cursorSprite;
	MyFuncsXTK::Sprite m_strreplaySprite;
	MyFuncsXTK::Sprite m_strtitleSprite;
	MyFuncsXTK::Sprite m_numshootSprite;
	MyFuncsXTK::Sprite m_numdamageSprite;
	MyFuncsXTK::Sprite m_numdefeatSprite;
	MyFuncsXTK::Sprite m_strshootSprite;
	MyFuncsXTK::Sprite m_strdamageSprite;
	MyFuncsXTK::Sprite m_strdefeatSprite;
	MyFuncsXTK::Sprite m_back1Sprite;
	MyFuncsXTK::Sprite m_back2Sprite;
	MyFuncsXTK::Sprite m_changesceneSprite;

	//std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

	//プレイヤーのデータ
	int m_playerShootCount;
	int m_playerDamageCount;
	int m_playerDefeatCount;

	//デバッグカメラ
	//std::unique_ptr<DebugCamera> m_debugcamera;

	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//カメラのオブジェクト
	std::unique_ptr<Camera>m_camera;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	//自機パーツ
	enum PARTS
	{
		PARTS_FIGHTER,//機体
		PARTS_R_WING,//右翼
		PARTS_L_WING,//左翼

		PARTS_NUM
	};

	std::vector<Obj3d> m_Obj;


	bool m_changeScene;

};

