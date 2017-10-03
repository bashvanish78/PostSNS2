
#include "ResultScene.h"
#include "GameScene.h"
#include "pch.h"

#include "ADX2Le.h"
#include "Resources\Music\CueSheet_0.h"

#pragma comment(lib, "cri_ware_pcx86_LE_import.lib")

SceneState* ResultScene::pState = nullptr;
//std::unique_ptr<SceneState> pState;

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace MyFuncsXTK;

using Microsoft::WRL::ComPtr;

class SceneState;

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
	//ADX2Le::Finalize();
}

SceneState * ResultScene::GetInstance()
{
	if (pState == nullptr)
		pState = new ResultScene();
	//pState = std::make_unique();
	return pState;
}

void ResultScene::Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
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
	Tick(pManager, keyboard);

	//SceneUpdate();
	//SceneRender();
}

void ResultScene::SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Initialize(m_window, m_outputWidth, m_outputHeight, keyboard);
}

void ResultScene::SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	//�J������蒼��
	m_camera.reset();
	m_camera = std::make_unique<Camera>(m_outputWidth, m_outputHeight);

	//3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	//BGM�Đ�
	ADX2Le::StopBgm();
	ADX2Le::PlayBgm(CRI_CUESHEET_0_RESULTBGMLOOP);

	m_changeScene = false;

	//�X�v���C�g�̏�����
	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);
}

void ResultScene::SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Update(m_timer, pManager, keyboard);
}

void ResultScene::SceneRender()
{
	Render();
}

void ResultScene::Dispose()
{
	if (pState != nullptr)
		delete pState;
}

//
// Result.cpp
//

// Initialize the Direct3D resources required to run.
//������(�Q�[����ʂ̉����c��������)
void ResultScene::Initialize(HWND& window, int& width, int& height, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	//�E�B���h�E�̐ݒ�
	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	//�E�B���h�E�̐ݒ�
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

	//�V�X�e���I�ȏ�����
	//CreateDevice();

	//CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	////////////////////////////
	//�e�평�����͂��̕ӂɏ���//
	////////////////////////////

	m_gameHistory = false;

	//�J��������
	m_camera = std::make_unique<Camera>(m_outputWidth, m_outputHeight);
	//�J�����ɃL�[�{�[�h���Z�b�g
	//m_camera->SetKeyBoard(keyboard.get());

	//3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	//�ˉe�s������
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//�ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//�G�t�F�N�g�t�@�N�g���쐬
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	//�e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");

	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_FIGHTER].LoadModel(L"Resources/Fighter.cmo");
	m_Obj[PARTS_R_WING].LoadModel(L"Resources/R_WING.cmo");
	m_Obj[PARTS_L_WING].LoadModel(L"Resources/L_WING.cmo");

	//�e�q�֌W�̍\�z(�q�p�[���ɐe��ݒ�)
	m_Obj[PARTS_R_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_L_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);

	//�X�P�[���̎w��
	m_Obj[PARTS_FIGHTER].SetScale(Vector3(2.4f));

	//���W�̂���w��
	m_Obj[PARTS_FIGHTER].SetTrans(Vector3(0, 0, 1));

	m_changeScene = false;

	//�X�v���C�g�o�b�a
	m_spriteBatch = std::make_shared<SpriteBatch>(m_d3dContext.Get());

	m_chooseGame = true;

	m_playerShootCount = 0;
	m_playerDamageCount = 0;
	m_playerDefeatCount = 0;

	//ACF�t�@�C���ӂ��݂���
	ADX2Le::Initialize("Resources/Music/Shooting3D.acf");
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb", "Resources/Music/CueSheet_0.awb");

	ADX2Le::StopBgm();
	ADX2Le::PlayBgm(CRI_CUESHEET_0_RESULTBGMLOOP);

	//�X�v���C�g�̏�����
	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);

	//�X�v���C�g�̏�����,�ݒ�
	m_cursorSprite.LoadTextureSetAll(L"Resources/cursor.png", Vector2(32.f, 32.f), Vector2(1.f, 1.f), Vector2(800 / 2 - 100, 600 / 2 + 160));
	m_strreplaySprite.LoadTextureSetAll(L"Resources/strReplay.png", Vector2(128, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160));
	m_strtitleSprite.LoadTextureSetAll(L"Resources/strTitle.png", Vector2(128, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160 + 40));
	m_numshootSprite.LoadTextureSetAll(L"Resources/number.png", Vector2(64, 48), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8), FloatConvertToRECT(0.f, 0.f, 64.f, 48.f));
	m_numdamageSprite.LoadTextureSetAll(L"Resources/number.png", Vector2(64, 48), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8 + 80), FloatConvertToRECT(0.f, 0.f, 64.f, 48.f));
	m_numdefeatSprite.LoadTextureSetAll(L"Resources/number.png", Vector2(64, 48), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8 + 160), FloatConvertToRECT(0.f, 0.f, 64.f, 48.f));
	m_strshootSprite.LoadTextureSetAll(L"Resources/strShoot.png", Vector2(214, 64), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3, 8 + 64 + 8));
	m_strdamageSprite.LoadTextureSetAll(L"Resources/strDamage.png", Vector2(214, 64), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3, 8 + 64 + 8 + 80));
	m_strdefeatSprite.LoadTextureSetAll(L"Resources/strDefeat.png", Vector2(214, 64), Vector2(1.f, 1.f), Vector2(128 + 24 + 32 * 3, 8 + 64 + 8 + 160));
	m_back1Sprite.LoadTextureSetAll(L"Resources/resultBack1.png", Vector2(800, 600), Vector2(1.f, 1.f), Vector2(800 / 2.f, 600 / 2.f));
	m_back2Sprite.LoadTextureSetAll(L"Resources/resultBack2.png", Vector2(800, 600), Vector2(1.f, 1.f), Vector2(800 / 2.f, 600 / 2.f));


	m_changesceneSprite.LoadTextureSetAll(L"Resources/changeScene.png", Vector2(1200, 600), Vector2(1.f, 1.f), Vector2(800 / 2.f, 600 / 2.f));
}

// Executes the basic game loop.
//Update��Render�Ăяo��
void ResultScene::Tick(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	m_timer.Tick([&]()
	{
		Update(m_timer, pManager, keyboard);
	});

	Render();
}


// Updates the world.
void ResultScene::Update(DX::StepTimer const& timer, SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	//////////////////////////
	//���t���[���̏���������//
	//////////////////////////

	ADX2Le::Update();

	//�L�[�{�[�h�̏�Ԏ擾
	Keyboard::State kb = keyboard->GetState();

	m_keyboardTracker.Update(kb);

	//�Q�[���ɖ߂鏀��
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_chooseGame && m_changeScene == false)
	{
		ADX2Le::StopBgm();
		ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
		m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));
		m_changeScene = true;
	}

	//�^�C�g���ɖ߂鏀��
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_chooseGame == false && m_changeScene == false)
	{
		ADX2Le::StopBgm();
		ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
		m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));
		m_changeScene = true;
	}

	m_camera->Update();
	m_view = m_camera->GetView();
	m_proj = m_camera->GetProjection();

	//�퓬�@�̍X�V
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	//�퓬�@�̉�]
	m_Obj[PARTS_FIGHTER].SetRot(Vector3(0.f, 0.01f, 0.f) + m_Obj[PARTS_FIGHTER].GetRot());

	//�I��
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


	m_changesceneSprite.SetPos(Vector2(m_changesceneSprite.GetPos().x - 20, m_changesceneSprite.GetPos().y));

	//�V�[���؂�ւ�
	if (m_changeScene == true)
	{
		if (m_changesceneSprite.GetPos().x < 800 / 2)
		{
			ADX2Le::StopBgm();
			if (m_chooseGame)
			{
				pManager->ChangeState(GameScene::GetInstance());
				pManager->GetState()->SceneInitialize2(keyboard);
			}
			else
			{
				pManager->ChangeState(TitleScene::GetInstance());
				pManager->GetState()->SceneInitialize2(keyboard);
			}
		}
	}
}

// Draws the scene.
void ResultScene::Render()
{
	//// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	//�L�����o�X�̏�����
	Clear();

	// TODO: Add your rendering code here.

	//////////////////
	//�`�揈��������//
	//////////////////

	//Opaque = �h��Ԃ��i�s����)
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//���s�����g��Ȃ�
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//�J�����O���Ȃ�(�\���ǂ����ł��`��)
	m_d3dContext->RSSetState(m_states->Wireframe());

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	//�ݒ�𔽉f
	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_back1Sprite.Draw();

	//�v���C���[�̕`��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	//�X�v���C�g�̕`��
	m_back2Sprite.Draw();

	m_cursorSprite.Draw();

	m_strreplaySprite.Draw();
	m_strtitleSprite.Draw();

	m_numshootSprite.DrawNum(m_playerShootCount);
	m_numdamageSprite.DrawNum(m_playerDamageCount);
	m_numdefeatSprite.DrawNum(m_playerDefeatCount);

	m_strshootSprite.Draw();
	m_strdamageSprite.Draw();
	m_strdefeatSprite.Draw();

	m_changesceneSprite.Draw();

	//��ʂɔ��f
	Present();
}

// Helper method to clear the back buffers.
void ResultScene::Clear()
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
void ResultScene::Present()
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
void ResultScene::OnActivated()
{
	// TODO: Game is becoming active window.
}

void ResultScene::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void ResultScene::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void ResultScene::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void ResultScene::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void ResultScene::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void ResultScene::CreateDevice()
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
void ResultScene::CreateResources()
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

void ResultScene::OnDeviceLost()
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

void ResultScene::SetPlayerDataOnResult(Player* player)
{
	m_playerShootCount = player->GetShootTimeCount();
	m_playerDamageCount = player->GetDamageCount();
	m_playerDefeatCount = player->GetDefeatCount();

	//���l�`��̂���C��

	m_numshootSprite.SetPos(Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8));

	if (m_playerDamageCount == 0)
	{
		m_numdamageSprite.SetPos(Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120 + 16, 8 + 64 + 8 + 80));
	}
	else
	{
		m_numdamageSprite.SetPos(Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8 + 80));
	}

	if (m_playerDefeatCount == 0)
	{
		m_numdefeatSprite.SetPos(Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120 + 16, 8 + 64 + 8 + 160));
	}
	else
	{
		m_numdefeatSprite.SetPos(Vector2(128 + 24 + 32 * 3 + 128 + 64 * 2 + 120, 8 + 64 + 8 + 160));
	}
}
