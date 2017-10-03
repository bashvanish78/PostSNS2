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

	//変数追加が可能

	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugcamera;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//天球モデル
	Obj3d m_ObjSkyDome;


	LandShape m_LandShape;

	//キャタピラのモデル
	std::unique_ptr<DirectX::Model> m_modelCaterpiller;
	//キャタピラのモデル
	std::unique_ptr<DirectX::Model> m_modelCaterpiller2;

	//ティーポットのワールド行列
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	//キャタピラのワールド行列
	DirectX::SimpleMath::Matrix m_worldCaterpiller;
	//キャタピラのワールド行列
	DirectX::SimpleMath::Matrix m_worldCaterpiller2;

	float m_distance[20];
	int m_count;

	//std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//自機の座標
	float tank_rot;
	DirectX::SimpleMath::Vector3 tank_pos;


	//カメラのオブジェクト
	std::unique_ptr<FollowCamera>m_camera;

	std::unique_ptr<Player>m_Player;

	std::vector<std::unique_ptr<Enemy>>m_Enemies;

	std::vector<std::unique_ptr<Mine>>m_Mines;

	std::unique_ptr<Boss>m_Boss;

	//エフェクトマネージャー
	ModelEffectManager* m_ModelEffectManager;

	//スプライトバッヂ
	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//スプライトのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cleartexture;

	//敵の数
	int m_enemyNum;

	//デバッグ切り替え
	void ChangeDebug();

	//スプライト
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

	//スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	//文字列
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

	//ボス出現の描画
	int m_bossAppearsCount;
	const int m_bossAppearsTime = 90;

	//ポーズ
	bool m_pauseState;

	//ポーズ中メニュー
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
