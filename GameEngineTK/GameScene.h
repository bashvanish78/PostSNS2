#pragma once

#include "DebugCamera.h"
#include "FollowCamera.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>
#include "Obj3d.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Mine.h"
#include "ModelEffect.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include <WICTextureLoader.h>
#include "LandShape.h"
#include "Boss.h"
#include "MyFuncsXTK.h"

#include "TitleScene.h"

//#include "ScreenShot.h"

//#include "Sprite.h"

class SceneManager;
class SceneState;

class GameScene :
	public SceneState
{
private:
	static SceneState* pState;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture2;
public:
	GameScene();
	~GameScene();

	static SceneState* GetInstance();

	void Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard);
	void SceneRender();

	static void Dispose();

	void SetPlayerDataOnResult(Player* player);

	/// <summary>
	/// Game
	/// </summary>
public:

	//Game();

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
	
	void Initialize(HWND& window, int& width, int& height, std::shared_ptr<DirectX::Keyboard> keyboard);
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

	//�ϐ��ǉ����\

	//�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�ėp�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugcamera;

	//�G�t�F�N�g�t�@�N�g��
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//�V�����f��
	Obj3d m_ObjSkyDome;


	LandShape m_LandShape;

	//�L���^�s���̃��f��
	std::unique_ptr<DirectX::Model> m_modelCaterpiller;
	//�L���^�s���̃��f��
	std::unique_ptr<DirectX::Model> m_modelCaterpiller2;

	//�e�B�[�|�b�g�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	//�L���^�s���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldCaterpiller;
	//�L���^�s���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldCaterpiller2;

	float m_distance[20];
	int m_count;

	//std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//���@�̍��W
	float tank_rot;
	DirectX::SimpleMath::Vector3 tank_pos;


	//�J�����̃I�u�W�F�N�g
	std::unique_ptr<FollowCamera>m_camera;

	std::unique_ptr<Player>m_Player;

	std::vector<std::unique_ptr<Enemy>>m_Enemies;

	std::vector<std::unique_ptr<Mine>>m_Mines;

	std::unique_ptr<Boss>m_Boss;

	//�G�t�F�N�g�}�l�[�W���[
	ModelEffectManager* m_ModelEffectManager;

	//�X�v���C�g�o�b�a
	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//�X�v���C�g�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cleartexture;

	//�G�̐�
	int m_enemyNum;

	//�f�o�b�O�؂�ւ�
	void ChangeDebug();

	//�X�v���C�g
	MyFuncsXTK::Sprite m_clearSprite;
	MyFuncsXTK::Sprite m_bulletSprite;
	MyFuncsXTK::Sprite m_bulletrelSprite;
	MyFuncsXTK::Sprite m_dangerSprite;
	MyFuncsXTK::Sprite m_perfectSprite;
	MyFuncsXTK::Sprite m_bossappearsSprite;
	MyFuncsXTK::Sprite m_numSpriteAdvance;
	MyFuncsXTK::Sprite m_numSpriteEnemyNum;
	MyFuncsXTK::Sprite m_pointSprite;
	MyFuncsXTK::Sprite m_enemyleftSprite;
	MyFuncsXTK::Sprite m_advancedSprite;
	MyFuncsXTK::Sprite m_pauseSprite;
	MyFuncsXTK::Sprite m_cursorSprite;
	MyFuncsXTK::Sprite m_strresumeSprite;
	MyFuncsXTK::Sprite m_strrestartSprite;
	MyFuncsXTK::Sprite m_strtitleSprite;
	MyFuncsXTK::Sprite m_changesceneSprite;

	MyFuncsXTK::Sprite m_SSSprite;

	//�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	//������
	std::wstring m_str;
	std::wstring m_str2;

	void Player2Enemy();
	void Player2Boss();
	void Player2Mine();
	void Bullet2Enemy();

	void Bullet2Boss();
	void BossBullet2Player();

	void Player2Land();
	void StringAdvance();
	void StringEnemyNum();

	void GameRestart();

	//void DrawNum(int x, int y, int n);
	//void DrawNum(int x, int y, float n);

	bool m_playerdeathhistory;

	bool m_bossDeath;
	bool m_bossState;

	//�{�X�o���̕`��
	int m_bossAppearsCount;
	const int m_bossAppearsTime = 90;

	//�|�[�Y
	bool m_pauseState;

	//�|�[�Y�����j���[
	int m_pauseMenuState;
	bool m_returnGame;

	int m_clearFlameCount;
	const int m_clearFlameTime = 200;

	int m_defaultEnemynum;

	bool m_resultHistory;

	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_changescenetexture;

	//DirectX::SimpleMath::Vector2 m_changeScenePos;

	bool m_changeScene;

	void ScreenShot();

	int m_numScreenShot;

	const int m_SSUIvisibleTime = 90;
	int m_SSUIvisibleCount;
	bool m_SSUIstate;

	MyFuncsXTK::Sprite m_UISprite;

	std::vector<std::wstring> get_file_path_in_dir(const std::wstring& dir_name, const std::wstring& extension) noexcept(false);

	int FindFile();

	std::string WStringToString(std::wstring oWString);

	void SelectScreenShot();

	int m_screenShotID;
	bool m_loadedScreenshotFirst;
	bool m_screenShotDrawState;

	MyFuncsXTK::Sprite m_SSSpriteBack;
	MyFuncsXTK::Sprite m_SSVUIArrow1;
	MyFuncsXTK::Sprite m_SSVUIArrow2;
	MyFuncsXTK::Sprite m_SSVUIEnter;

	int m_screenShotNum;

	void DrawSSID();

	//ScreenShot a;
};
