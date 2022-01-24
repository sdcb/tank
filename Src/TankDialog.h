#pragma once

namespace Tank
{
	class TankDialog final
	{
	public:
		static bool Confirm(const wchar_t* content, const wchar_t* title = nullptr);
	};
}