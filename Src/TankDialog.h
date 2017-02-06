#pragma once

namespace Tank
{
	class TankDialog final
	{
	public:
		static bool Confirm(wchar_t * const content, wchar_t * const title = nullptr);
	};
}