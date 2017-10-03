
//�X�N���[���V���b�g�̂��ߕK�v
#include <ScreenGrab.h>
#include <wincodec.h>
#include <wincodecsdk.h>

//�t�@�C���̗L�����m���߂邽�ߕK�v
#include <Shlwapi.h>
#include <direct.h>

//�X�N���[���V���b�g�̃t�@�C�����ɐ��l
#include <string.h>
#include <atlstr.h>  
#include <wchar.h>
#include <time.h>

#pragma comment(lib, "WindowsCodecs.lib")

#include "GameScene.h"
#include "ResultScene.h"

#include "pch.h"

#include "ADX2Le.h"
#include "Resources\Music\CueSheet_0.h"



#include <Effects.h>

//#pragma comment(lib, "WindowsCodecs.lib")

#pragma comment(lib, "cri_ware_pcx86_LE_import.lib")

SceneState* GameScene::pState = nullptr;
//std::unique_ptr<SceneState> pState;

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace MyFuncsXTK;

using Microsoft::WRL::ComPtr;

GameScene::GameScene()
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
}

GameScene::~GameScene()
{
	//ADX2Le::Finalize();
}

SceneState * GameScene::GetInstance()
{
	if (pState == nullptr)
		pState = new GameScene();
		//pState = std::make_unique();
		return pState;
}

void GameScene::Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
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

void GameScene::SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Initialize(m_window,m_outputWidth,m_outputHeight,keyboard);
}

void GameScene::SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	GameRestart();

	m_camera->SetPlayer(m_Player.get());
	//�J�����ɃL�[�{�[�h���Z�b�g
	m_camera->SetKeyBoard(keyboard.get());

	//3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	if (ADX2Le::IsPauseBgm())
	{
		ADX2Le::PauseBgm();
	}
	
	ADX2Le::StopBgm();
	ADX2Le::PlayBgm(CRI_CUESHEET_0_BGMLOOP);

	m_pauseMenuState = 0;

	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);
}

void GameScene::SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	Update(m_timer,pManager,keyboard);
}

void GameScene::SceneRender()
{
	//Render(keyboard);
}


void GameScene::Dispose()
{
	if (pState != nullptr)
		delete pState;
}

void GameScene::SetPlayerDataOnResult(Player* player)
{
}

//
// Game.cpp
//


// Initialization and management
// Initialize the Direct3D resources required to run.
//������(�Q�[����ʂ̉����c��������)
void GameScene::Initialize(HWND& window, int& width, int& height, std::shared_ptr<DirectX::Keyboard> keyboard)
{
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

	//OnDeviceLost();

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

	////keyboard = std::make_unique<Keyboard>();

	m_resultHistory = false;

	//�J��������
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//�J�����ɃL�[�{�[�h���Z�b�g
	m_camera->SetKeyBoard(keyboard.get());

	//3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	//�n�ʓ�����̐ݒ�<LandShape�̏������̐ݒ�
	LandShapeCommonDef lscDef;
	lscDef.pDevice = m_d3dDevice.Get();
	lscDef.pDeviceContext = m_d3dContext.Get();
	lscDef.pCamera = m_camera.get();
	//�n�ʓ�����̋��ʏ�����
	LandShape::InitializeCommon(lscDef);


	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	//�ˉe�s������
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		(float)m_outputWidth, (float)m_outputHeight, 0.f, 0.f, 1.f));
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

	m_debugcamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//�G�t�F�N�g�t�@�N�g���쐬
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
	//���f���쐬
	m_ObjSkyDome.LoadModel(L"Resources/skydome.cmo");

	//�n�`�f�[�^�̓ǂݍ���
	m_LandShape.Initialize(L"ground200m", L"ground200m");

	m_count = 0;

	tank_rot = 0.0f;

	tank_pos = Vector3(0.0f, 0.0f, 0.0f);

	//�v���C���[�̐���
	m_Player = std::make_unique<Player>(keyboard.get());
	m_Player->Initialize();
	m_Player->SetTrans(Vector3(0, 3, 0));

	m_camera->SetPlayer(m_Player.get());

	// �G�̐���
	m_enemyNum = rand() % 10 + 1;
	//m_enemyNum = 0;
	m_Enemies.resize(m_enemyNum);

	for (int i = 0; i < m_enemyNum; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>();
		m_Enemies[i]->Initialize();

		int transx = (rand() % 40 + 2) * MinusOrPlus();
		int transz = (rand() % 40 + 2) * MinusOrPlus();

		m_Enemies[i]->SetTrans(Vector3((float)transx, 0.f, (float)transz));
	}

	m_Boss = std::make_unique<Boss>();
	m_Boss->Initialize();

	// �@���̐���
	int mineNum = rand() % 30 + 70;
	m_Mines.resize(mineNum);

	for (int i = 0; i < mineNum; i++)
	{
		m_Mines[i] = std::make_unique<Mine>();
		m_Mines[i]->Initialize();

		int transx = (rand() % 100 + 2) * MinusOrPlus();
		int transz = (rand() % 100 + 2) * MinusOrPlus();

		float scale = rand() % 50 / 10.0f + 1;

		m_Mines[i]->SetTrans(Vector3((float)transx, (float)(rand() % 20), (float)transz));
		m_Mines[i]->SetScale(Vector3(scale, scale, scale));
	}

	//�G�t�F�N�g�}�l�[�W���[�̎擾
	m_ModelEffectManager = ModelEffectManager::getInstance();

	//�X�v���C�g�o�b�a
	m_spriteBatch = std::make_shared<SpriteBatch>(m_d3dContext.Get());

	//�X�v���C�g�t�H���g
	m_spriteFont = std::make_unique<SpriteFont>(m_d3dDevice.Get(), L"Resources/myfile.spritefont");

	m_playerdeathhistory = false;

	//ACF�t�@�C���ӂ��݂���
	ADX2Le::Initialize("Resources/Music/Shooting3D.acf");
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb", "Resources/Music/CueSheet_0.awb");

	ADX2Le::PlayBgm(CRI_CUESHEET_0_BGMLOOP);

	//�t���O��������

	m_bossDeath = false;
	m_bossState = false;

	m_bossAppearsCount = 0;

	m_pauseState = false;

	m_pauseMenuState = 0;

	m_returnGame = false;

	m_changeScene = false;

	m_clearFlameCount = 0;

	m_defaultEnemynum = m_enemyNum;

	MyFuncsXTK::Sprite::InitializeStatic(m_spriteBatch, m_d3dDevice);

	//�X�v���C�g�̓ǂݍ��݁A�ݒ�
	m_clearSprite.LoadTextureSetAll(L"Resources/clear.png", Vector2(256, 64), Vector2(1.f, 1.f), Vector2(800 / 2, 600 / 2));
	m_bulletSprite.LoadTextureSetAll(L"Resources/bullet.png", Vector2(26, 128), Vector2(1.f, 1.f), Vector2(800 - 26 / 2, 600 - 128 / 2));
	m_bulletrelSprite.LoadTextureSetAll(L"Resources/bulletrel.png", Vector2(26, 128), Vector2(1.f, 1.f), Vector2(800 - 26 / 2, 600 - 128 / 2));
	m_dangerSprite.LoadTextureSetAll(L"Resources/danger.png", Vector2(640, 164), Vector2(1.f, 1.f), Vector2(725 - 640 / 2, 600 - 64 / 2));
	m_perfectSprite.LoadTextureSetAll(L"Resources/perfect.png", Vector2(320, 64), Vector2(1.f, 1.f), Vector2(800 / 2, 600 / 2 + 64));
	m_bossappearsSprite.LoadTextureSetAll(L"Resources/bossappears.png", Vector2(640, 164), Vector2(1.f, 1.f), Vector2(725 - 640 / 2, 600 - 64 / 2 - 64));
	m_numSpriteAdvance.LoadTextureSetAll(L"Resources/number.png", Vector2(64, 48), Vector2(0.5f, 0.5f), Vector2(8 + 96 + 32 * 3, 8 + 64 / 2), FloatConvertToRECT(0.f, 0.f, 64.f, 48.f));
	m_numSpriteEnemyNum.LoadTextureSetAll(L"Resources/number.png", Vector2(64, 48), Vector2(0.5f, 0.5f), Vector2(128 + 24 + 32 * 3, 8 + 64 + 8), FloatConvertToRECT(0.f, 0.f, 64.f, 48.f));
	m_pointSprite.LoadTextureSetAll(L"Resources/point.png", Vector2(64, 48), Vector2(0.5f, 0.5f), Vector2(0, 0));
	m_enemyleftSprite.LoadTextureSetAll(L"Resources/enemyleft.png", Vector2(128, 64), Vector2(1.f, 1.f), Vector2(8 + 128 / 2, 8 + 64 + 8));
	m_advancedSprite.LoadTextureSetAll(L"Resources/advanced.png", Vector2(96, 64), Vector2(1.f, 1.f), Vector2(8 + 96 / 2, 8 + 64 / 2));
	m_pauseSprite.LoadTextureSetAll(L"Resources/pause.png", Vector2(256, 64), Vector2(1.f, 1.f), Vector2(800 / 2, 600 / 2 - 150));
	m_cursorSprite.LoadTextureSetAll(L"Resources/cursor.png", Vector2(32, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100, 600 / 2 + 160 + (0 * 32)));
	m_strresumeSprite.LoadTextureSetAll(L"Resources/strResume.png", Vector2(128, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160 + (0 * 32)));
	m_strrestartSprite.LoadTextureSetAll(L"Resources/strRestart.png", Vector2(128, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160 + (1 * 32)));
	m_strtitleSprite.LoadTextureSetAll(L"Resources/strTitle.png", Vector2(128, 32), Vector2(1.f, 1.f), Vector2(800 / 2 - 100 + 40 + 64, 600 / 2 + 160 + (2 * 32)));
	m_changesceneSprite.LoadTextureSetAll(L"Resources/changeScene.png", Vector2(1200, 600), Vector2(1.f, 1.f), Vector2(800 / 2.f, 600 / 2.f));
	m_UISprite.LoadTextureSetAll(L"Resources/SSUI.png", Vector2(256.f, 48.f), Vector2(1.f, 1.f), Vector2(128.f, 600.f - 24.f));

	m_numScreenShot = 0;

	m_screenShotID = 0;

	m_SSUIvisibleCount = 0;
	m_SSUIstate = false;

	m_SSSprite.SetImageSize(Vector2(800, 600));
	m_SSSprite.SetScale(Vector2(0.35f, 0.35f));
	m_SSSprite.SetPos(Vector2(400,300));

	m_SSSpriteBack.LoadTextureSetAll(L"Resources/resultBack1.png", Vector2(800, 600), Vector2(0.38f, 0.38f), Vector2(400, 300));

	m_SSVUIArrow1.LoadTextureSetAll(L"Resources/SSVUIArrow.png", Vector2(48, 48), Vector2(1, 1), Vector2(100, 300));
	m_SSVUIArrow2.LoadTextureSetAll(L"Resources/SSVUIArrow2.png", Vector2(48, 48), Vector2(1, 1), Vector2(700, 300));
	m_SSVUIEnter.LoadTextureSetAll(L"Resources/SSVUIEnter.png", Vector2(139, 48), Vector2(1, 1), Vector2(70, 600 - 24 - 48));

	m_loadedScreenshotFirst = false;
	m_screenShotDrawState = true;

	m_screenShotNum = 0;
}


// Executes the basic game loop.
//Update��Render�Ăяo��
void GameScene::Tick(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	m_timer.Tick([&]()
	{
		Update(m_timer,pManager,keyboard);
	});

	Render();
}


// Updates the world.
void GameScene::Update(DX::StepTimer const& timer, SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	//////////////////////////
	//���t���[���̏���������//
	//////////////////////////

	//�L�[�{�[�h�̏�Ԏ擾
	Keyboard::State kb = keyboard->GetState();
	m_keyboardTracker.Update(kb);
	
	if (m_SSUIstate)
	{
		m_SSUIvisibleCount++;
		if (m_SSUIvisibleCount > m_SSUIvisibleTime)
		{
			m_SSUIvisibleCount = 0;
			m_SSUIstate = false;
		}
	}

	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::B) && m_SSUIstate)
	{
		m_SSUIstate = false;
	}
	m_changesceneSprite.SetPos(Vector2(m_changesceneSprite.GetPos().x - 20, m_changesceneSprite.GetPos().y));

	//�V�[���؂�ւ�
	if (m_changeScene == true)
	{
		if (m_changesceneSprite.GetPos().x < 800 / 2)
		{
			if (m_enemyNum <= 0 && m_bossDeath)
			{
				ADX2Le::StopBgm();
				pManager->ChangeState(ResultScene::GetInstance());

				if (m_resultHistory)
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
					m_resultHistory = true;
				}
				pManager->GetState()->SetPlayerDataOnResult(m_Player.get());
			}

			if (m_pauseMenuState == 1)
			{
				GameRestart();
			}

			if (m_pauseMenuState == 2)
			{
				pManager->ChangeState(TitleScene::GetInstance());
				pManager->GetState()->SceneInitialize2(keyboard);
			}
		}
	}

	/// <summary>
	/// �|�[�Y����Ă���Ƃ��̏���
	/// </summary>
	if (m_pauseState)
	{
		SelectScreenShot();

		//�|�[�Y���Ƀ|�[�Y�{�^���Ŗ߂�
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::P) && m_changeScene == false)
		{
			m_pauseState = false;
			ADX2Le::PauseBgm();
			//ADX2Le::Stop();
			//ADX2Le::Play(CRI_CUESHEET_0_BGMLOOP);
			//ADX2Le::GetPlayer()->PauseByID(CRI_CUESHEET_0_BGMLOOP);
			//ADX2Le::PauseById(CRI_CUESHEET_0_BGMLOOP);
			m_returnGame = true;
			m_pauseMenuState = 0;
			return;
		}

		//�J�[�\���ړ�
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Up) && m_changeScene == false)
		{
			ADX2Le::PlaySe(CRI_CUESHEET_0_SELECT);
			if (m_pauseMenuState <= 0)
			{
				m_pauseMenuState = 2;
			}
			else
			{
				m_pauseMenuState--;
			}
		}

		//�J�[�\���ړ�
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Down) && m_changeScene == false)
		{
			ADX2Le::PlaySe(CRI_CUESHEET_0_SELECT);
			if (m_pauseMenuState >= 2)
			{
				m_pauseMenuState = 0;
			}
			else
			{
				m_pauseMenuState++;
			}
		}

		m_cursorSprite.SetPos(Vector2((float)(800 / 2 - 100), (float)(600 / 2 + 160 + (m_pauseMenuState * 32))));

		/// <summary>
		/// �I������Ă�����̂ɂ���ď�����ς���
		/// </summary>
		if (m_pauseMenuState == 0)
		{
			//�v���C�ɖ߂�
			if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_changeScene == false)
			{
				m_pauseState = false;
				ADX2Le::PauseBgm();
				ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
				//ADX2Le::Play(CRI_CUESHEET_0_BGMLOOP);
				//ADX2Le::GetPlayer()->PauseByID(CRI_CUESHEET_0_BGMLOOP);
				//ADX2Le::PauseById(CRI_CUESHEET_0_BGMLOOP);
				m_returnGame = true;
				return;
			}
		}
		else if (m_pauseMenuState == 1)
		{
			//GameScene�����Z�b�g
			if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_changeScene == false)
			{
				m_changeScene = true;
				m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));

				ADX2Le::PauseBgm();
				ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
				//ADX2Le::Play(CRI_CUESHEET_0_BGMLOOP);

				return;
			}
		}
		else
		{
			//�^�C�g���ɖ߂�
			if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_changeScene == false)
			{
				ADX2Le::PlaySe(CRI_CUESHEET_0_DICIDE);
				m_changeScene = true;
				m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));

				return;
				//ADX2Le::Stop();

			}
		}

		return;

	}
	else
	{
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::P) && m_changeScene == false && m_bossDeath == false)
		{
			m_pauseState = true;
			//ADX2Le::GetPlayer()->PauseByID(CRI_CUESHEET_0_BGMLOOP);
			ADX2Le::PauseBgm();
			ADX2Le::PlaySe(CRI_CUESHEET_0_DESTRUCTION1);
			//ADX2Le::PauseById(CRI_CUESHEET_0_BGMLOOP);
		}
	}

	//m_keyboardTracker.Reset();

	//m_keyboardTracker.Update(kb);

	ADX2Le::Update();

	m_debugcamera->Update();

	m_count++;

	static int landshapeCount = 0;

	m_Player->SetGameSceneResume(m_returnGame);

	m_Player->Update();

	//�G�̍X�V
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
		it != m_Enemies.end();
		it++)
	{
		Enemy* enemy = it->get();
		enemy->Update();

		//(*it)->Update();
	}

	//�@���̍X�V
	for (std::vector<std::unique_ptr<Mine>>::iterator it = m_Mines.begin();
		it != m_Mines.end();
		it++)
	{
		Mine* mine = it->get();
		mine->Update();

		//(*it)->Update();
	}

	//�����蔻��

	Player2Enemy();

	Player2Mine();

	Player2Boss();

	Bullet2Enemy();

	Bullet2Boss();

	BossBullet2Player();

	//�G���^�[�������ꂽ��f�o�b�O�؂�ւ�
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Enter))
	{
		ChangeDebug();
	}

	{//���@�ɒǏ]����J����
	 //�J�����X�V

		m_camera->Update();
		m_view = m_camera->GetView();
		m_proj = m_camera->GetProjection();
	}

	//�n�ʂ̈ʒu����
	landshapeCount++;
	if (landshapeCount >= 20)
	{
		landshapeCount = 0;
		m_LandShape.SetTrans(Vector3(m_Player->GetTrans().x, 0, m_Player->GetTrans().z));
	}

	m_ObjSkyDome.SetTrans(Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y, m_Player->GetTrans().z));

	m_LandShape.Update();
	m_ObjSkyDome.Update();

	//�G�t�F�N�g�̃A�b�v�f�[�g
	m_ModelEffectManager->Update();

	//�G����S���|������{�X�o��
	if (m_bossDeath == false && m_enemyNum <= 0 && m_bossState == false)
	{
		m_Boss->SetTrans(Vector3(m_Player->GetTrans().x + (rand() % 50 + 2) * MinusOrPlus(), m_Player->GetTrans().y + 6.0f, m_Player->GetTrans().z + (rand() % 50 + 2) * MinusOrPlus()));
		m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Boss->GetTrans()), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(10, 10, 10), Vector3(5, 5, 5));
		m_Boss->SetHP(m_Boss->GetDefaultHP());
		m_bossState = true;
	}

	if (m_bossState)
	{
		m_Boss->Update();
	}

	//�n�ʂƃv���C���[�̔���

	Player2Land();

	//���l�̍X�V

	StringAdvance();

	StringEnemyNum();

	//�v���C���[�����񂾂�PARFECT�\���t���O����
	if (m_Player->GetDeathState() && !m_bossDeath)
	{
		m_playerdeathhistory = true;
	}
	if (m_Player->GetWingState() != 0 && !m_bossDeath)
	{
		m_playerdeathhistory = true;
	}

	//�S���|������N���A
	if (m_enemyNum <= 0 && m_bossDeath && m_changeScene == false)
	{
		m_clearFlameCount++;
		if (m_clearFlameTime <= m_clearFlameCount)
		{
			m_clearFlameCount = 0;
			m_changeScene = true;
			m_changesceneSprite.SetPos(Vector2(1200, m_changesceneSprite.GetPos().y));
		}
	}

	m_returnGame = false;
}

// Draws the scene.
void GameScene::Render()
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
	//m_d3dContext�ɕ`�揈����˂�����
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

	//���f���̕`��
	//��@
	m_ObjSkyDome.Draw();
	//�n��
	m_LandShape.Draw();

	//�v���C���[�̕`��
	m_Player->Draw();

	//�G�̕`��
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
		it != m_Enemies.end();
		it++)
	{
		(*it)->Draw();
	}

	//�@���̕`��
	for (std::vector<std::unique_ptr<Mine>>::iterator it = m_Mines.begin();
		it != m_Mines.end();
		it++)
	{
		(*it)->Draw();
	}

	//�G�����S�ł��ă{�X������ł��Ȃ���Ε`��
	if (m_bossDeath == false && m_enemyNum <= 0)
	{
		m_Boss->Draw();
	}


	//�G�t�F�N�g�̕`��
	m_ModelEffectManager->Draw();

	//�N���A�̃X�v���C�g�\��
	if (m_enemyNum <= 0 && m_bossDeath)
	{
		m_clearSprite.Draw();
		if (m_playerdeathhistory == false)
		{
			m_perfectSprite.Draw();
		}
	}

	//�e�ۂ̃X�v���C�g�\��
	if (!m_Player->GetFireFlag())
	{
		m_bulletSprite.Draw();
	}
	else
	{
		m_bulletrelSprite.Draw();
	}

	//�v���C���[������ł��Ȃ���΍��x��\��
	if (!m_Player->GetDeathState())
	{
		m_numSpriteAdvance.DrawNum(m_Player->GetTrans().y - m_Player->GetCollisionNodeBody().Radius, 2, m_pointSprite);
		m_advancedSprite.Draw();

		if (m_Player->GetTrans().y - m_Player->GetCollisionNodeBody().Radius < 1)
		{
			m_dangerSprite.Draw();
		}
	}

	//�G���\��
	if (m_bossState == false)
	{
		m_numSpriteEnemyNum.DrawNum(m_enemyNum);
		m_enemyleftSprite.Draw();
	}

	//�|�[�Y��
	if (m_pauseState)
	{
		m_pauseSprite.Draw();
		m_cursorSprite.Draw();
		m_strresumeSprite.Draw();
		m_strrestartSprite.Draw();
		m_strtitleSprite.Draw();

		if (m_loadedScreenshotFirst && m_screenShotDrawState)
		{
			m_SSSpriteBack.Draw();
			m_SSSprite.Draw();
			m_SSVUIArrow1.Draw();
			m_SSVUIArrow2.Draw();
			m_SSVUIEnter.Draw();

			DrawSSID();
		}
	}

	//�{�X���o�������Ƃ�
	if (m_bossState)
	{
		m_bossAppearsCount++;
		if (m_bossAppearsCount <= m_bossAppearsTime)
		{
			m_bossappearsSprite.Draw();
		}
	}

	if (m_SSUIstate)
	{
		m_UISprite.Draw();
	}


	m_changesceneSprite.Draw();

	//��ʂɔ��f
	Present();

	//B�������ꂽ��X�N���[���V���b�g(��ʂɔ��f����Ă����ԂłȂ��Ɛ^���ÂɂȂ�)
	ScreenShot();
	//a.TakeScreenShot(m_keyboardTracker,m_d3dContext,m_swapChain);
}

// Helper method to clear the back buffers.
void GameScene::Clear()
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
void GameScene::Present()
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
void GameScene::OnActivated()
{
	// TODO: Game is becoming active window.
}

void GameScene::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void GameScene::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void GameScene::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void GameScene::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void GameScene::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void GameScene::CreateDevice()
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
void GameScene::CreateResources()
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

void GameScene::OnDeviceLost()
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

void GameScene::ChangeDebug()
{
	m_Player->ChangeDebug();
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
		it != m_Enemies.end();
		it++)
	{
		Enemy* enemy = it->get();
		enemy->ChangeDebug();
	}

	for (std::vector<std::unique_ptr<Mine>>::iterator it = m_Mines.begin();
		it != m_Mines.end();
		it++)
	{
		Mine* mine = it->get();
		mine->ChangeDebug();
	}

	m_Boss->ChangeDebug();

}

void GameScene::Player2Enemy()
{
	//���@�ƓG�̂����蔻��
	{
		const Sphere& bodySphere = m_Player->GetCollisionNodeBody();

		//�G�̐���������������
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
			it != m_Enemies.end();
			)
		{
			Enemy* enemy = it->get();
			const Sphere& enemySphere = enemy->GetCollisionNodeBody();

			//���菈��(����ł��Ȃ��Ƃ��A���G��Ԃ���Ȃ��Ƃ��̂�)
			if (CheckSphere2Sphere(bodySphere, enemySphere) && !m_Player->GetDeathState() && !m_Player->GetInvincibleState())
			{
				if (m_Player->GetWingState() == 0)
				{
					m_Player->Damage();
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector3(5, 5, 5));
					ADX2Le::PlaySe(CRI_CUESHEET_0_DESTRUCTION1);
					break;
				}
				else
				{
					//�G�t�F�N�g���o���A�v���C���[���E��
					m_Player->SetDeathState(true);
					//�N���A���Ă��Ȃ���ԂȂ玀�S�񐔃J�E���g
					if(!m_bossDeath) m_Player->PlusDeathCount();
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
					ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
					break;
				}
			}
			else
			{
				//�����Ȃ������ꍇ�C�e���[�^�[�����߂�
				it++;
			}
		}
	}
}

void GameScene::Player2Boss()
{
	//���@�ƃ{�X�̂����蔻��
	if (m_bossDeath == false && m_bossState)
	{
		const Sphere& bodySphere = m_Player->GetCollisionNodeBody();

		const Sphere& bossSphere = m_Boss->GetCollisionNodeBody();

		//���菈��(����ł��Ȃ��Ƃ��A���G��Ԃ���Ȃ��Ƃ��̂�)
		if (CheckSphere2Sphere(bodySphere, bossSphere) && !m_Player->GetDeathState() && !m_Player->GetInvincibleState())
		{
			if (m_Player->GetWingState() == 0)
			{
				m_Player->Damage();
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector3(5, 5, 5));
				ADX2Le::PlaySe(CRI_CUESHEET_0_DESTRUCTION1);
			}
			else
			{
				//�G�t�F�N�g���o���A�v���C���[���E��
				m_Player->SetDeathState(true);
				//�N���A���Ă��Ȃ���ԂȂ玀�S�񐔃J�E���g
				if (!m_bossDeath) m_Player->PlusDeathCount();
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
				ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
			}
		}
	}
}

void GameScene::Player2Mine()
{
	//���@�Ƌ@���̂����蔻��
	{
		const Sphere& bodySphere = m_Player->GetCollisionNodeBody();

		//�@���̐���������������
		for (std::vector<std::unique_ptr<Mine>>::iterator it = m_Mines.begin();
			it != m_Mines.end();
			)
		{
			Mine* mine = it->get();
			const Sphere& mineSphere = mine->GetCollisionNodeBody();

			//���菈��(����ł��Ȃ��Ƃ��A���G��Ԃ���Ȃ��Ƃ��̂�)
			if (CheckSphere2Sphere(bodySphere, mineSphere) && !m_Player->GetDeathState() && !m_Player->GetInvincibleState())
			{
				if (m_Player->GetWingState() == 0)
				{
					m_Player->Damage();
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector3(5, 5, 5));
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3((*it)->GetTrans().x, (*it)->GetTrans().y + 1, (*it)->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(10, 10, 10), Vector3(25, 25, 25));
					ADX2Le::PlaySe(CRI_CUESHEET_0_DESTRUCTION1);
					ADX2Le::PlaySe(CRI_CUESHEET_0_EXPLOSION1);
					it = m_Mines.erase(it);
				}
				else
				{
					//�G�t�F�N�g���o���A�v���C���[���E��
					m_Player->SetDeathState(true);
					//�N���A���Ă��Ȃ���ԂȂ玀�S�񐔃J�E���g
					if (!m_bossDeath) m_Player->PlusDeathCount();
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
					m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3((*it)->GetTrans().x, (*it)->GetTrans().y + 1, (*it)->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(10, 10, 10), Vector3(25, 25, 25));
					ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
					//�G���E��,�������v�f�̎��̗v�f���h���C�e���[�^�[
					it = m_Mines.erase(it);
				}
			}
			else
			{
				//�����Ȃ������ꍇ�C�e���[�^�[�����߂�
				it++;
			}
		}
	}
}

void GameScene::Bullet2Enemy()
{
	//�e�ۂƓG�̂����蔻��
	{
		const Sphere& bulletSphere = m_Player->GetCollisionNodeBullet();

		//�G�̐���������������
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
			it != m_Enemies.end();
			)
		{
			Enemy* enemy = it->get();
			const Sphere& enemySphere = enemy->GetCollisionNodeBody();

			//���菈��(�e�𔭎˂����Ƃ��̂�)
			if (CheckSphere2Sphere(bulletSphere, enemySphere) && m_Player->GetFireFlag())
			{
				//�G�t�F�N�g���o��
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3((*it)->GetTrans().x, (*it)->GetTrans().y + 1, (*it)->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
				//�G���E��,�������v�f�̎��̗v�f���h���C�e���[�^�[
				it = m_Enemies.erase(it);
				//�G�̎c�������炷
				m_enemyNum--;
				ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
			}
			else
			{
				//�����Ȃ������ꍇ�C�e���[�^�[�����߂�
				it++;
			}
		}
	}
}

void GameScene::Bullet2Boss()
{
	//�e�ۂƓG�̂����蔻��
	if (m_bossDeath == false && m_bossState)
	{
		const Sphere& bulletSphere = m_Player->GetCollisionNodeBullet();

		const Sphere& bossSphere = m_Boss->GetCollisionNodeBody();

		//���菈��(�e�𔭎˂����Ƃ��̂�)
		if (CheckSphere2Sphere(bulletSphere, bossSphere) && m_Player->GetFireFlag() && !m_Boss->GetInvincibleState())
		{
			//�{�X�̗̑͂��c���Ă���Ƃ��Ǝc���Ă��Ȃ��Ƃ��ŕ���
			if (m_Boss->GetHP() - 1 >= 0 + 1)
			{
				//�G�t�F�N�g���o��
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 12, Vector3(m_Boss->GetTrans().x, m_Boss->GetTrans().y + 1, m_Boss->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(30, 30, 30));
				//�{�X�Ƀ_���[�W
				m_Boss->Damage();

				ADX2Le::PlaySe(CRI_CUESHEET_0_EXPLOSION1);
			}
			else
			{
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Boss->GetTrans().x, m_Boss->GetTrans().y + 1, m_Boss->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
				ADX2Le::StopBgm();
				ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
				ADX2Le::PlayBgm(CRI_CUESHEET_0_CLEARBGM);
				m_bossDeath = true;
			}

		}
	}
}

void GameScene::BossBullet2Player()
{
	//���@�ƃ{�X�̂����蔻��
	if (m_bossDeath == false && m_bossState)
	{
		const Sphere& bodySphere = m_Player->GetCollisionNodeBody();

		const Sphere& bossBulletSphere = m_Boss->GetCollisionNodeBullet();

		//���菈��(����ł��Ȃ��Ƃ��A���G��Ԃ���Ȃ��Ƃ��̂�)
		if (CheckSphere2Sphere(bodySphere, bossBulletSphere) && !m_Player->GetDeathState() && !m_Player->GetInvincibleState())
		{
			if (m_Player->GetWingState() == 0)
			{
				m_Player->Damage();
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector3(5, 5, 5));
				ADX2Le::PlaySe(CRI_CUESHEET_0_DESTRUCTION1);
			}
			else
			{
				//�G�t�F�N�g���o���A�v���C���[���E��
				m_Player->SetDeathState(true);
				//�N���A���Ă��Ȃ���ԂȂ玀�S�񐔃J�E���g
				if (!m_bossDeath) m_Player->PlusDeathCount();
				m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
				ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
			}
		}
	}
}

void GameScene::Player2Land()
{
	//�n�ʂƎ����̔���
	{
		Sphere sphere = m_Player->GetCollisionNodeBody();
		//���@�̃��[���h���W
		Vector3 trans = m_Player->GetTrans();
		//������v���C���[�ւ̃x�N�g��
		//Vector3 sphere2player = trans - sphere.Center;
		//�߂荞�ݔr�˃x�N�g��
		Vector3 reject;

		//���ƒn�`�̂����蔻��
		if (m_LandShape.IntersectSphere(sphere, &reject) && !m_Player->GetDeathState())
		{
			//�n�ʂɐG���ƃv���C���[�͎���
			m_Player->SetDeathState(true);
			//�N���A���Ă��Ȃ���ԂȂ玀�S�񐔃J�E���g
			if (!m_bossDeath) m_Player->PlusDeathCount();
			m_ModelEffectManager->Entry(L"Resources/HitEffect2.cmo", 30, Vector3(m_Player->GetTrans().x, m_Player->GetTrans().y + 1, m_Player->GetTrans().z), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(100, 100, 100));
			ADX2Le::PlaySe(CRI_CUESHEET_0_BOOM);
		}
		//���[���h�s��X�V
		//m_Player->Calc();
	}
}

//���x�\���̕���
void GameScene::StringAdvance()
{
	//��������
	std::wstringstream ss;
	//�X�g�����O�X�g���[���ɏo��
	ss << L"Advanced:" << m_Player->GetTrans().y - m_Player->GetCollisionNodeBody().Radius;
	//�X�g�����O�X�g���[�����當������擾
	m_str = ss.str();
}

//�G�̎c���\���\���̕���
void GameScene::StringEnemyNum()
{
	//��������
	std::wstringstream ss;
	//�X�g�����O�X�g���[���ɏo��
	ss << L"Enemy left:" << m_enemyNum;
	//ss << L"Enemy left:" << m_Boss->GetHP();
	//�X�g�����O�X�g���[�����當������擾
	m_str2 = ss.str();
}

void GameScene::GameRestart()
{
	m_Player->SetTrans(Vector3(0, 3, 0));

	// �G�̐���
	m_enemyNum = rand() % 10 + 1;
	//m_enemyNum = 0;
	m_Enemies.resize(m_enemyNum);

	for (int i = 0; i < m_enemyNum; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>();
		m_Enemies[i]->Initialize();

		int transx = (rand() % 40 + 10) * MinusOrPlus();
		int transz = (rand() % 40 + 10) * MinusOrPlus();

		m_Enemies[i]->SetTrans(Vector3((float)transx, 0.f, (float)transz));
	}

	// �@���̐���
	int mineNum = rand() % 30 + 70;
	m_Mines.resize(mineNum);

	for (int i = 0; i < mineNum; i++)
	{
		m_Mines[i] = std::make_unique<Mine>();
		m_Mines[i]->Initialize();

		int transx = (rand() % 100 + 2) * MinusOrPlus();
		int transz = (rand() % 100 + 2) * MinusOrPlus();

		float scale = rand() % 50 / 10.0f + 1;

		m_Mines[i]->SetTrans(Vector3((float)transx, (float)(rand() % 20), (float)transz));
		m_Mines[i]->SetScale(Vector3(scale, scale, scale));
	}

	m_playerdeathhistory = false;

	m_Player->SetWingState(0);
	m_Player->SetRot(Vector3(0, 0, 0));

	m_Player->SetDeathState(false);
	m_Player->ResetRespawnCount();

	m_bossDeath = false;
	m_bossState = false;

	m_bossAppearsCount = 0;

	m_pauseState = false;
	m_clearFlameCount = 0;

	m_pauseMenuState = 0;

	m_defaultEnemynum = m_enemyNum;

	m_Player->PlayerDataReset();

	m_changeScene = false;


	m_SSUIvisibleCount = 0;
	m_SSUIstate = false;
	m_loadedScreenshotFirst = false;
	m_screenShotDrawState = true;
}

void GameScene::ScreenShot()
{
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::B))
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
		if (SUCCEEDED(hr))
		{
			_mkdir("ScreenShots");

			std::vector<std::wstring> file_names;

			file_names = get_file_path_in_dir(L"ScreenShots", L"bmp");

			std::string strnameFull;
			std::string strnameNum;
			int nameNum;
			bool numState = false;

			int filenum = FindFile();

			if (filenum > 2)
			{
				for (std::vector<std::wstring>::iterator it = file_names.begin();
					it != file_names.end();
					it++)
				{
					strnameFull = WStringToString((*it));
					strnameNum = strnameFull.substr(0, 2);
					nameNum = std::stoi(strnameNum);
					//m_numScreenShot += 1;
					//if (m_numScreenShot != nameNum)
					//{
					//	m_numScreenShot = nameNum + 1;
					//	break;
					//}
				}
			}
			else
			{
				m_numScreenShot = 0;
			}


			if (filenum > 2)
			{
				m_numScreenShot += nameNum + 1;
			}

			CString strNum;

			wchar_t buf[500];

			//wchar_t * num = _itow(m_numScreenShot,num,10);

			time_t now = time(NULL);
			struct tm *pnow = localtime(&now);

			//wcscat_s(buf,500, L"ScreenShots\\");
			//swprintf_s(buf, 500, L"ScreenShots\\");

			bool numstate = false;

			if (m_numScreenShot < 10)
			{
				numstate = true;
			}
			//wcscat_s(buf, 500, L"0");
			//wcscat_s(buf,500, num);
			//swprintf_s(buf, 500, L"%d",m_numScreenShot);

			//wcscat_s(buf,500, L"_");

			//wchar_t * time = _itow(pnow->tm_year + 1900, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_year + 1900);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//time = _itow(pnow->tm_mon + 1, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_mon + 1);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//time = _itow(pnow->tm_mday, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_mday);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//time = _itow(pnow->tm_hour, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_hour);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//time = _itow(pnow->tm_min, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_min);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//time = _itow(pnow->tm_sec, time, 10);
			//swprintf_s(buf, 500, L"%d", pnow->tm_sec);

			//wcscat_s(buf,500, time);
			//wcscat_s(buf,500, L"_");

			//wcscat_s(buf,500, L".bmp");

			if (numstate)
			{
				swprintf_s(buf, 500, L"ScreenShots\\0%d_%d_%d_%d_%d_%d_%d_.bmp", m_numScreenShot, pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);

			}
			else
			{
				swprintf_s(buf, 500, L"ScreenShots\\%d_%d_%d_%d_%d_%d_%d_.bmp", m_numScreenShot, pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
			}

			hr = SaveWICTextureToFile(m_d3dContext.Get(), backBuffer.Get(),
				GUID_ContainerFormatBmp, buf,
				&GUID_WICPixelFormat16bppBGR565);
			
			m_SSUIstate = true;

			m_numScreenShot = 0;

		}

		DX::ThrowIfFailed(hr);
	}

}

std::vector<std::wstring> GameScene::get_file_path_in_dir(const std::wstring & dir_name, const std::wstring & extension) noexcept(false)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;//defined at Windwos.h
	std::vector<std::wstring> file_names;

	//�g���q�̐ݒ�
	std::wstring search_name = dir_name + L"\\*." + extension;

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		//throw std::runtime_error("file not found");
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			file_names.push_back(win32fd.cFileName);
			//printf("%s\n", file_names.back().c_str());

		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return file_names;
}

int GameScene::FindFile()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR *target = L"ScreenShots\\*";
	int fileCount = 0;

	hFind = FindFirstFile(target, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			++fileCount;
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}
	return fileCount;
}


/*
wstring��string�֕ϊ�
*/
std::string GameScene::WStringToString
(
	std::wstring oWString
)
{
	// wstring �� SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char *)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpMultiByte;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

void GameScene::SelectScreenShot()
{
	m_screenShotDrawState = true;

	int filenum = FindFile();

	if (filenum > 2)
	{
		std::vector<std::wstring> file_names;
		file_names = get_file_path_in_dir(L"ScreenShots", L"bmp");

		wchar_t buf[500] = L"ScreenShots\\";

		if (m_loadedScreenshotFirst == false)
		{
			std::wstring test = file_names[m_screenShotID];

			wcscat_s(buf,500, test.c_str());
			m_loadedScreenshotFirst = true;
			m_SSSprite.LoadTexture(buf);
		}

		m_screenShotNum = std::distance(file_names.begin(), file_names.end()) - 1;

		//�J�[�\���ړ�
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Right) && m_changeScene == false)
		{
			m_screenShotID += 1;
			if(m_screenShotID > m_screenShotNum)
			{
				m_screenShotID = 0;
			}

			std::wstring test2 = file_names[m_screenShotID];
			wcscat_s(buf,500, test2.c_str());
			m_screenShotDrawState = false;
			m_SSSprite.LoadTexture(buf);
		}

		//�J�[�\���ړ�
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Left) && m_changeScene == false)
		{
			m_screenShotID -= 1;
			if (m_screenShotID < 0)
			{
				m_screenShotID = m_screenShotNum;
			}

			std::wstring test3 = file_names[m_screenShotID];
			wcscat_s(buf,500, test3.c_str());
			m_screenShotDrawState = false;
			m_SSSprite.LoadTexture(buf);
		}

		//�c�C�[�g
		if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Enter) && m_changeScene == false)
		{
			//�c�C�[�g�̏����������ł���
			//file_names[m_screenShotID]�����ݑI�����Ă���摜



		}

		//m_SSSprite.Draw();
	}


}

void GameScene::DrawSSID()
{
	wchar_t buf[256];

	swprintf_s(buf, 256, L"%d / %d", m_screenShotID+1, m_screenShotNum+1);

	m_spriteBatch->Begin();

	m_spriteFont->DrawString(m_spriteBatch.get(),buf,Vector2(700,16));

	m_spriteBatch->End();
}
