#pragma once

#include "SceneManager.h"

#include "StepTimer.h"
#include <Model.h>
#include "Player.h"

class SceneManager;
class Player;

class SceneState {
public:
	virtual ~SceneState() {
		
		_depthStencilView.Reset();
		_renderTargetView.Reset();
		_swapChain1.Reset();
		_swapChain.Reset();
		_d3dContext1.Reset();
		_d3dContext.Reset();
		_d3dDevice1.Reset();
		_d3dDevice.Reset();

		
	};

	virtual void Execute(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard) = 0;
	virtual void SceneInitialize(std::shared_ptr<DirectX::Keyboard> keyboard) = 0;
	virtual void SceneInitialize2(std::shared_ptr<DirectX::Keyboard> keyboard) = 0;
	virtual void SceneUpdate(SceneManager* pManager, std::shared_ptr<DirectX::Keyboard> keyboard) = 0;
	virtual void SceneRender() = 0;

	void SetWindow(const HWND window) { _window = window; }
	void SetOutputWidth(const int outputWidth) { _outputWidth = outputWidth; }
	void SetOutputHeight(const int outputHeight) { _outputHeight = outputHeight; }
	void SetFeatureLevel(const D3D_FEATURE_LEVEL featurelevel) { _featureLevel = featurelevel; }
	void SetD3dDevice(const Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice) { _d3dDevice = d3dDevice; }
	void SetD3dDevice1(const Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice1) { _d3dDevice1 = d3dDevice1; }
	void SetD3dContext(const Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext) { _d3dContext = d3dContext; }
	void SetD3dContext1(const Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext1) { _d3dContext1 = d3dContext1; }
	void SetSwapChain(const Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) { _swapChain = swapChain; }
	void SetSwapChain1(const Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1) { _swapChain1 = swapChain1; }
	void SetRenderTargetView(const Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView) { _renderTargetView = renderTargetView; }
	void SetDepthStencilView(const Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView) { _depthStencilView = depthStencilView; }
	void SetTimer(const DX::StepTimer timer) { _timer = timer; }

	//リザルトでのみ使用
	virtual void SetPlayerDataOnResult(Player* player) = 0;

protected:
	// Device resources.
	HWND                                            _window;
	int                                             _outputWidth;
	int                                             _outputHeight;

	D3D_FEATURE_LEVEL                               _featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>            _d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11Device1>           _d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     _d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    _d3dContext1;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          _swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         _swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  _renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  _depthStencilView;

	// Rendering loop timer.
	DX::StepTimer                                   _timer;
};