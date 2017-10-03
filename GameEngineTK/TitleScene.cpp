
#include "TitleScene.h"
#include "GameScene.h"
//#include "ViewScreenShotScene.h"
#include "pch.h"

//#include "ScreenShot.h"

#include "ADX2Le.h"
#include "Resources\Music\CueSheet_0.h"

#pragma comment(lib, "cri_ware_pcx86_LE_import.lib")

SceneState* TitleScene::pState = nullptr;
//std::unique_ptr<SceneState> pState;

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace MyFuncsXTK;

using Microsoft::WRL::ComPtr;


TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	//ADX2Le::Finalize();
}

SceneState * TitleScene::GetInstance()
{
	if (pState == nullptr)
		pState = new TitleScene();
	//pState = std::make_unique();
	return pState;
}

void TitleScene::Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	// Device resources.
	m_window = _window;
	m_outputWidth = _outputWidth;
	m_outputHeight = _outputHeight;

	m_featureLevel = _featureLevel;
	m_d3dDevice = _d3dDevice;
	m_d3dDevice1 = _d3dDevice1;
	m_d3dContext = _d3dContext;
	m_d3dContext1 = _d3dContext1;

	m_swapChain = _swapChain;
	m_swapChain1 = _swapChain1;
	m_renderTargetView = _renderTargetView;
	m_depthStencilView = _depthStencilView;
	m_timer = _timer;
	Tick(pManager,keyboard);

	//SceneUpdate();
	//SceneRender();
}

void TitleScene::SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Initialize(m_window, m_outputWidth, m_outputHeight,keyboard);
}

void TitleScene::SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	//ポーズ中なら解除
	if (ADX2Le::IsPauseBgm())
	{
		ADX2Le::PauseBgm();
	}

	//BGM再生
	ADX2Le::StopBgm();
	ADX2Le::PlayBgm(CRI_CUESHEET_0_TITLEBGMLOOP);

	//スプライト初期化
	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);

	m_changeScene = false;

	//m_SSSprite.LoadTexture(L"ScreenShots/ScreenShots0.bmp");
}

void TitleScene::SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Update(m_timer,pManager,keyboard);
}

void TitleScene::SceneRender()
{
	Render();
}

void TitleScene::Dispose()
{
	if (pState != nullptr)
		delete pState;
}

void TitleScene::SetPlayerDataOnResult(Player* player)
{
}

//
// Title.cpp
//

// Initialize the Direct3D resources required to run.
//初期化(ゲーム画面の横幅縦幅を引数)
void TitleScene::Initialize(HWND& window, int& width, int& height, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	//ウィンドウの設定
	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	//ウィンドウの設定
	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	// Device resources.
	m_window = _window;
	m_outputWidth = _outputWidth;
	m_outputHeight = _outputHeight;

	m_featureLevel = _featureLevel;
	m_d3dDevice = _d3dDevice;
	m_d3dDevice1 = _d3dDevice1;
	m_d3dContext = _d3dContext;
	m_d3dContext1 = _d3dContext1;

	m_swapChain = _swapChain;
	m_swapChain1 = _swapChain1;
	m_renderTargetView = _renderTargetView;
	m_depthStencilView = _depthStencilView;
	m_timer = _timer;

	//システム的な初期化
	//CreateDevice();

	//CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	////////////////////////////
	//各種初期化はこの辺に書く//
	////////////////////////////
	 
	m_gameHistory = false;

	//スプライトバッヂ
	m_spriteBatch = std::make_shared<SpriteBatch>(m_d3dContext.Get());

	m_chooseGame = true;

	m_changeScene = false;

	//ACFファイル読み込み
	ADX2Le::Initialize("Resources/Music/Shooting3D.acf");
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb", "Resources/Music/CueSheet_0.awb");

	ADX2Le::PlayBgm(CRI_CUESHEET_0_TITLEBGMLOOP);

	//スプライト初期化
	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);

	//スプライトの設定
	m_titleSprite.LoadTextureSetAll(L"Resources/title.png", Vector2(800.f, 600.f), Vector2(1.f, 1.f), Vector2(800.f / 2, 600.f / 2));
	m_cursorSprite.LoadTextureSetAll(L"Resources/cursor.png", Vector2(32.f, 32.f), Vector2(1.f, 1.f), Vector2(800 / 2 - 100, 600 / 2 + 160));
	m_strstartSprite.LoadTextureSetAll(L"Resources/strStart.png", Vector2(128.f, 32.f), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160));
	m_strexitSprite.LoadTextureSetAll(L"Resources/strExit.png", Vector2(128.f, 32.f), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160 + 40));
	m_titlelogoSprite.LoadTextureSetAll(L"Resources/titleLogo.png", Vector2(800, 200), Vector2(1.f, 1.f), Vector2(800 / 2, 200 / 2 + 30));
	m_changesceneSprite.LoadTextureSetAll(L"Resources/changeScene.png", Vector2(1200, 600), Vector2(1.f, 1.f), Vector2(800 / 2.f, 600 / 2.f));

	//m_SSSprite.LoadTextureSetAll(L"ScreenShots/ScreenShots0.bmp", Vector2(800, 600), Vector2(0.75f, 0.75f), Vector2(800 / 2.f, 600 / 2.f));


}

// Executes the basic game loop.
//UpdateとRender呼び出し
void TitleScene::Tick(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	m_timer.Tick([&]()
	{
		Update(m_timer,pManager,keyboard);
	});

	Render();
}


// Updates the world.
void TitleScene::Update(DX::StepTimer const& timer, SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	//////////////////////////
	//毎フレームの処理を書く//
	//////////////////////////

	ADX2Le::Update();

	//キーボードの状態取得
	Keyboard::State kb = keyboard->GetState();

	m_keyboardTracker.Update(kb);

	//ゲームシーンに移行する準備
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_chooseGame && m_changeScene == false)
	{
		ADX2Le::StopBgm();
		ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
		m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));
		m_changeScene = true;
	}

	//アプリ終了
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_chooseGame == false && m_changeScene == false)
	{
		ExitGame();
	}

	//if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Tab))
	//{
	//	pManager->ChangeState(ViewScreenShotScene::GetInstance());

	//	pManager->GetState()->SetWindow(m_window);
	//	pManager->GetState()->SetOutputWidth(m_outputWidth);
	//	pManager->GetState()->SetOutputHeight(m_outputHeight);
	//	pManager->GetState()->SetFeatureLevel(m_featureLevel);
	//	pManager->GetState()->SetD3dDevice(m_d3dDevice);
	//	pManager->GetState()->SetD3dDevice1(m_d3dDevice1);
	//	pManager->GetState()->SetD3dContext(m_d3dContext);
	//	pManager->GetState()->SetD3dContext1(m_d3dContext1);
	//	pManager->GetState()->SetSwapChain(m_swapChain);
	//	pManager->GetState()->SetSwapChain1(m_swapChain1);
	//	pManager->GetState()->SetRenderTargetView(m_renderTargetView);
	//	pManager->GetState()->SetDepthStencilView(m_depthStencilView);
	//	pManager->GetState()->SetTimer(m_timer);

	//	pManager->GetState()->SceneInitialize(keyboard);
	//}

	//選択肢の移動
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Up) || m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Down) && m_changeScene == false)
	{
		ADX2Le::PlaySe(CRI_CUESHEET_0_SELECT);
		if (m_chooseGame)
		{
			m_chooseGame = false;
			m_cursorSprite.SetPos(Vector2(800 / 2 - 100, 600 / 2 + 160 + 40));
		}
		else
		{
			m_chooseGame = true;
			m_cursorSprite.SetPos(Vector2(800 / 2 - 100, 600 / 2 + 160));
		}
	}

	//カーソルのスプライト移動
	m_changesceneSprite.SetPos(Vector2(m_changesceneSprite.GetPos().x - 20, m_changesceneSprite.GetPos().y));

	//シーン遷移
	if (m_changeScene == true)
	{
		if (m_changesceneSprite.GetPos().x < 800 / 2)
		{
			pManager->ChangeState(GameScene::GetInstance());
			if (m_gameHistory)
			{
				pManager->GetState()->SceneInitialize2(keyboard);
			}
			else
			{
				pManager->GetState()->SetWindow(m_window);
				pManager->GetState()->SetOutputWidth(m_outputWidth);
				pManager->GetState()->SetOutputHeight(m_outputHeight);
				pManager->GetState()->SetFeatureLevel(m_featureLevel);
				pManager->GetState()->SetD3dDevice(m_d3dDevice);
				pManager->GetState()->SetD3dDevice1(m_d3dDevice1);
				pManager->GetState()->SetD3dContext(m_d3dContext);
				pManager->GetState()->SetD3dContext1(m_d3dContext1);
				pManager->GetState()->SetSwapChain(m_swapChain);
				pManager->GetState()->SetSwapChain1(m_swapChain1);
				pManager->GetState()->SetRenderTargetView(m_renderTargetView);
				pManager->GetState()->SetDepthStencilView(m_depthStencilView);
				pManager->GetState()->SetTimer(m_timer);

				pManager->GetState()->SceneInitialize(keyboard);
				m_gameHistory = true;
			}
		}
	}
	

}

// Draws the scene.
void TitleScene::Render()
{
	//// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	//キャンバスの初期化
	Clear();

	// TODO: Add your rendering code here.

	//////////////////
	//描画処理を書く//
	//////////////////

	//スプライト描画
	m_titleSprite.Draw();

	m_cursorSprite.Draw();

	m_strstartSprite.Draw();

	m_strexitSprite.Draw();

	m_titlelogoSprite.Draw();

	m_changesceneSprite.Draw();

	//m_SSSprite.Draw();

	//画面に反映
	Present();

	//アプリ終了
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Enter) && m_chooseGame == false && m_changeScene == false)
	{
		//ScreenShot screen;
		//screen.Initialize();
		//screen.TakeScreenShot(m_d3dContext,m_swapChain);
	}
}

// Helper method to clear the back buffers.
void TitleScene::Clear()
{
	// Clear the views.
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void TitleScene::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void TitleScene::OnActivated()
{
	// TODO: Game is becoming active window.
}

void TitleScene::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void TitleScene::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void TitleScene::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void TitleScene::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void TitleScene::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void TitleScene::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	HRESULT hr = D3D11CreateDevice(
		nullptr,                                // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
		&m_featureLevel,                        // returns feature level of device created
		m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
	);

	if (hr == E_INVALIDARG)
	{
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			m_d3dDevice.ReleaseAndGetAddressOf(),
			&m_featureLevel,
			m_d3dContext.ReleaseAndGetAddressOf()
		);
	}

	DX::ThrowIfFailed(hr);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	// DirectX 11.1 if present
	if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
		(void)m_d3dContext.As(&m_d3dContext1);

	// TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void TitleScene::CreateResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
		{
			// DirectX 11.1 or later

			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
			fsSwapChainDesc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window.
			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				m_d3dDevice.Get(),
				m_window,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				m_swapChain1.ReleaseAndGetAddressOf()
			));

			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = m_window;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = TRUE;

			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
		}

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
}

void TitleScene::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain1.Reset();
	m_swapChain.Reset();
	m_d3dContext1.Reset();
	m_d3dContext.Reset();
	m_d3dDevice1.Reset();
	m_d3dDevice.Reset();

	CreateDevice();

	CreateResources();
}

