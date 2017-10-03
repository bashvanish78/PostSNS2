#pragma once

#include "TitleScene.h"
#include "SceneState.h"

class SceneState;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void ChangeState(SceneState* state);

	void Execute(std::shared_ptr<DirectX::Keyboard> keyboard);

	void Initialize(std::shared_ptr<DirectX::Keyboard> keyboard);
	void Update();
	void Render();

	SceneState* GetState() { return pCurrentState; }

	SceneState* pCurrentState;
};

