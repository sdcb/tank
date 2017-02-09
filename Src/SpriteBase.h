#pragma once

#include <functional>
#include "SpriteUnit.h"
#include <dx/dx.h>
#include <DirectXTK/Keyboard.h>

namespace DX
{
	class DeviceResources;
	class StepTimer;
}

namespace Tank
{
	class SpriteBase
	{
	public:
		typedef std::function<void(SpriteUnit unit, KennyKerr::Point2F topLeft)> DrawCall;

		SpriteBase(DX::DeviceResources * dxRes);

		virtual ~SpriteBase();
		virtual void Update(DX::StepTimer const * timer) = 0;
		virtual void Draw(DrawCall drawCall) = 0;

		// window events
		virtual void OnClick(KennyKerr::Point2F cursorPos);
		virtual void OnMouseMove(KennyKerr::Point2F cursorPos);
		virtual void OnKeyUp(DirectX::Keyboard::Keys key);

	protected:
		DX::DeviceResources * m_dxRes;
	};
}