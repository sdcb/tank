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
	class SpriteBase
	{
	public:
		typedef std::function<void(TankSpriteUnit unit, KennyKerr::Point2F topLeft)> DrawCall;
		SpriteBase(DrawCall drawCall);

		void SetTopLeft(KennyKerr::Point2F topLeft);
		KennyKerr::Point2F GetTopLeft() const;

		virtual TankSpriteUnit GetCurrentSprite() const = 0;
		virtual void Update(DX::StepTimer const & timer) = 0;
		virtual void Draw(DX::DeviceResources * dxRes) = 0;

	private:
		std::function<void(TankSpriteUnit unit, KennyKerr::Point2F topLeft)> m_drawCall;
		KennyKerr::Point2F m_topLeft;

	public:
		static void DrawOutline(const SpriteBase& sprite, DX::DeviceResources * device);
	};
}