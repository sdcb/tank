#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

struct GameBase : public DX::IDeviceNotify
{
public:
	GameBase();
	virtual ~GameBase();
	virtual void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();
	virtual void Update(DX::StepTimer const& timer);
	void Render();
	void Clear();
	virtual void Draw(KennyKerr::Direct2D::DeviceContext target);

	virtual void CreateDeviceResources();
	virtual void CreateWindowSizeResources();

	// Í¨¹ý IDeviceNotify ¼Ì³Ð
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();
	virtual void OnWindowSizeChanged(int width, int height);

	// Properties
	virtual void GetDefaultSize(int& width, int& height) const;

protected:
	// DirectX related device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;
};