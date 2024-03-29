//
// pch.h
// Header for standard system include files.
//

#pragma once
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0602
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = { 0 };
            sprintf_s(s_str, "Failure with HRESULT of %08X", result);
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

#include <DirectXTK/Audio.h>
#include <DirectXTK/CommonStates.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/GamePad.h>
#include <DirectXTK/GeometricPrimitive.h>
#include <DirectXTK/GraphicsMemory.h>
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/Model.h>
#include <DirectXTK/Mouse.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/ScreenGrab.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>

#include <unordered_map>
#include <string>
#include <array>
struct LOGFONTW;
#include <dx/dx.h>

#include <format>