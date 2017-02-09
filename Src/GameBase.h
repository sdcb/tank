#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBase.h"
#include "PreDefined.h"

namespace Tank
{
	class SpriteButton;
}

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

	virtual void OnClick(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnKeyUp(DirectX::Keyboard::Keys key);

	void DrawUnit(Tank::SpriteUnit id, KennyKerr::Point2F center);

	// Properties
	virtual void GetDefaultSize(int& width, int& height) const;

	// create new sprite button
	Tank::SpriteButton* NewSpriteButton(KennyKerr::Point2F topLeft, std::vector<Tank::SpriteUnit> unites);

protected:
	// DirectX related device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

private:
	const Tank::TankSpriteMapArray          m_tankSpriteMap;
	KennyKerr::Direct2D::Bitmap1            m_bmp;
	std::vector<std::unique_ptr<Tank::SpriteBase>> m_buttons;
};