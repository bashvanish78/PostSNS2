#pragma once

#include "MyFuncsXTK.h"
#include <SimpleMath.h>


#include "StepTimer.h"
#include "SceneState.h"

//#include "Sprite.h"

class SceneManager;
class SceneState;

class TitleScene : public SceneState
{
private:
	static SceneState* pState;
	
public:
	TitleScene();
	~TitleScene();

	static SceneState* GetInstance();

	void Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneRender();

	static void Dispose();

	void SetPlayerDataOnResult(Player* player);

	/// <summary>
	/// Title
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
	MyFuncsXTK::Sprite m_titleSprite;
	MyFuncsXTK::Sprite m_cursorSprite;
	MyFuncsXTK::Sprite m_strstartSprite;
	MyFuncsXTK::Sprite m_strexitSprite;
	MyFuncsXTK::Sprite m_titlelogoSprite;
	MyFuncsXTK::Sprite m_changesceneSprite;

	MyFuncsXTK::Sprite m_SSSprite;

	//std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

	DirectX::SimpleMath::Vector2 m_changeScenePos;

	bool m_changeScene;

};

