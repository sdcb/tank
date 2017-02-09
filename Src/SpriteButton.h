#pragma once

#include "PreDefined.h"
#include <functional>
#include <dx/dx.h>
#include "SpriteBase.h"

namespace DX
{
	class DeviceResources;
	class StepTimer;
}

namespace Tank
{
	struct ShortCut
	{
		bool Ctrl;
		bool Alt;
		bool Shift;
		DirectX::Keyboard::Keys Key;

		ShortCut();

		static ShortCut Parse(std::string str);
		static ShortCut Key_(DirectX::Keyboard::Keys key);
		static ShortCut Shift_(DirectX::Keyboard::Keys key);

		bool Match(bool ctrl, bool alt, bool shift, DirectX::Keyboard::Keys key) const;
	};

	class SpriteButton final: public SpriteBase
	{
	public:
		SpriteButton(
			KennyKerr::Point2F topLeft, 
			std::vector<SpriteUnit> sprites, 
			DX::DeviceResources * dxRes);
		~SpriteButton() override;

		// position: top left
		void SetTopLeft(KennyKerr::Point2F topLeft);
		KennyKerr::Point2F GetTopLeft() const;

		// current sprite
		SpriteUnit GetCurrentSprite() const;
		float GetCurrentSpriteSize() const;

		// update/draw
		void Update(DX::StepTimer const * timer) override;
		void Draw(DrawCall drawCall) override;

		// shortcut key
		ShortCut* GetShortCut() const;
		void SetShortCut(ShortCut shortCut);

		// visible
		void SetVisible(bool visible);
		bool Visible() const;

		// isX4
		void SetIsX4(bool isX4);
		bool IsX4() const;

		// outline
		void ShowOutline(bool showOutline);

		// click handler
		void SetClickHandler(std::function<void()> clickHandler);

		// window events
		void OnClick(KennyKerr::Point2F cursorPos) override;
		void OnMouseMove(KennyKerr::Point2F cursorPos) override;
		void OnKeyUp(DirectX::Keyboard::Keys key) override;

	private:
		// draw outline
		void DrawOutline();

		// top left
		KennyKerr::Point2F m_topLeft;

		// hover status/text
		bool         m_hover;

		// sprite related
		std::vector<SpriteUnit> m_sprites;
		size_t                  m_spriteId;

		// click handler
		std::function<void()> m_clickHandler;

		// short-cut
		std::unique_ptr<ShortCut> m_shortCut;

		// visible
		bool m_visible;
		bool m_isX4;
		bool m_showOutline;
	};
}