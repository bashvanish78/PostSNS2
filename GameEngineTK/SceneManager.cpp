
#include "SceneManager.h"

#include "GameScene.h"
#include "ResultScene.h"

SceneManager::SceneManager()
{
	pCurrentState = nullptr;
}

SceneManager::~SceneManager()
{
	TitleScene::Dispose();
	GameScene::Dispose();
	ResultScene::Dispose();
}

void SceneManager::ChangeState(SceneState * state)
{
	pCurrentState = state;
}

void SceneManager::Execute(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	pCurrentState->Execute(this,keyboard);
}

void SceneManager::Initialize(std::shared_ptr<DirectX::Keyboard> keyboard)
{
	pCurrentState->SceneInitialize(keyboard);
}

void SceneManager::Update()
{
	//pCurrentState->SceneUpdate(this);
}

void SceneManager::Render()
{
	pCurrentState->SceneRender();
}
