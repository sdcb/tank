#include "pch.h"
#include "TankDialog.h"

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


bool Tank::TankDialog::Confirm(wchar_t * const content, wchar_t * const title)
{
	int buttonSelected;
	DX::ThrowIfFailed(
		TaskDialog(nullptr, nullptr, title, title, content, TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON, nullptr, &buttonSelected));
	return buttonSelected == TDCBF_OK_BUTTON;
}
