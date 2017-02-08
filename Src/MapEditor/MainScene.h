//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "PreDefined.h"
#include "GameBase.h"
#include "MapStore.h"
#include "SpriteButton.h"
#define PROJECT_NAME L"Map Editor"

namespace Tank
{
	enum class SpriteType : unsigned char;
}

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public GameBase
{
public:
	Game();

	// IDeviceNotify
	virtual void OnDeviceLost() override;

private:

	void Update(DX::StepTimer const& timer) override;
	void Draw(KennyKerr::Direct2D::DeviceContext target) override;

	void CreateDeviceResources() override;
	void CreateWindowSizeResources() override;

	void DrawUnit(Tank::SpriteUnit id, KennyKerr::Point2F center);
	void DrawEnv(Tank::EnvType env, KennyKerr::Point2F topLeft);
	void DrawSprite(Tank::SpriteType sprite, KennyKerr::Point2F topLeft);

	// map functions
	void GoOffsetMap(int offset);
	void SaveMap();

	void DrawRight();
	void DrawBody();
	void DrawSpecialEnvironments();

	void SetupRightButtons();
	void SetupLeftButtons();

	Tank::SpriteButton* NewSpriteButton(KennyKerr::Point2F topLeft, std::vector<Tank::SpriteUnit> unites);

	// Device resources.
	KennyKerr::Direct2D::Bitmap1            m_bmp;
	std::function<KennyKerr::Direct2D::SolidColorBrush()> m_black, m_red;

	// text format
	std::function<KennyKerr::DirectWrite::TextFormat()>   m_textFormat;

	// selected environment type
	Tank::EnvType                           m_selectedEnv;
	bool                                    m_small;

	// scene array
	int                                     m_mapId;
	Tank::MapBody                           m_map;
	Tank::MapStore                          m_mapStore;
	std::array<Tank::EnvType, 6>            m_envSequence;

	// input states
	std::function<DirectX::Keyboard::State()> m_keyboardState;
	std::function<DirectX::Mouse::State()>    m_mouseState;
	std::function<KennyKerr::Point2F()>       m_mousePos;

	// sprite map
	const Tank::TankSpriteMapArray          m_tankSpriteMap;

	// scale
	D2D1::Matrix3x2F                        m_world;

	// sprite controls
	std::vector<std::unique_ptr<Tank::SpriteButton>> m_buttons;
	std::vector<Tank::SpriteButton*>                 m_envButtons;
	Tank::SpriteButton *m_rightBtn, *m_leftBtn, *m_pendingSaveBtn;

public:
	// window messages
	virtual void OnClick(int x, int y) override;
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnKeyUp(DirectX::Keyboard::Keys key) override;
};