#pragma once

#include "PreDefined.h"
#include <functional>
#include <dx/dx.h>

namespace DX
{
	class DeviceResources;
	class StepTimer;
}

namespace Tank
{
	class SpriteButton 
	{
	public:
		typedef std::function<void(TankSpriteUnit unit, KennyKerr::Point2F topLeft)> DrawCall;

		SpriteButton(
			KennyKerr::Point2F topLeft, 
			std::vector<TankSpriteUnit> sprites);

		// position: top left
		void SetTopLeft(KennyKerr::Point2F topLeft);
		KennyKerr::Point2F GetTopLeft() const;

		// current sprite
		TankSpriteUnit GetCurrentSprite() const;

		// update/draw
		void Update(DX::StepTimer const * timer);
		void Draw(DrawCall drawCall, DX::DeviceResources * dxRes);

		// shortcut key


		// click handler
		void SetClickHandler(std::function<void()> clickHandler);

		// window events
		void OnClick(KennyKerr::Point2F cursorPos);
		void OnMouseMove(KennyKerr::Point2F cursorPos);
		void OnKeyUp(DirectX::Keyboard::Keys key);

	private:
		// top left
		KennyKerr::Point2F m_topLeft;

		// hover status/text
		bool         m_hover;

		// sprite related
		std::vector<TankSpriteUnit> m_sprites;
		size_t                      m_spriteId;

		// click handler
		std::function<void()> m_clickHandler;

		// short-cut
		DirectX::Keyboard::Keys m_shortCutKey;

	public:
		static void DrawOutline(SpriteButton const * sprite, DX::DeviceResources * device);
	};
}