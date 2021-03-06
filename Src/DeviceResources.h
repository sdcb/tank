﻿//
// DeviceResources.h - A wrapper for the Direct3D 11 device and swapchain
//

#pragma once

namespace DX
{
    // Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
    interface IDeviceNotify
    {
        virtual void OnDeviceLost() = 0;
        virtual void OnDeviceRestored() = 0;
    };

    // Controls all the DirectX device resources.
    class DeviceResources
    {
    public:
        DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
                        DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
                        UINT backBufferCount = 2,
                        D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_9_1);

		void Update();

        void CreateDeviceResources();
        void CreateWindowSizeDependentResources();
        void SetWindow(HWND window, int width, int height);
        bool WindowSizeChanged(int width, int height);
        void HandleDeviceLost();
        void RegisterDeviceNotify(IDeviceNotify* deviceNotify) { m_deviceNotify = deviceNotify; }
        void Present();

        // Device Accessors.
        RECT GetOutputSize() const { return m_outputSize; }

        // Direct3D Accessors.
        ID3D11Device*           GetD3DDevice() const          { return m_d3dDevice.Get(); }
        ID3D11Device1*          GetD3DDevice1() const         { return m_d3dDevice1.Get(); }
        ID3D11DeviceContext*    GetD3DDeviceContext() const   { return m_d3dContext.Get(); }
        ID3D11DeviceContext1*   GetD3DDeviceContext1() const  { return m_d3dContext1.Get(); }
        IDXGISwapChain*         GetSwapChain() const          { return m_swapChain.Get(); }
        IDXGISwapChain1*        GetSwapChain1() const         { return m_swapChain1.Get(); }
        D3D_FEATURE_LEVEL       GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
        ID3D11Texture2D*        GetRenderTarget() const       { return m_renderTarget.Get(); }
        ID3D11Texture2D*        GetDepthStencil() const       { return m_depthStencil.Get(); }
        ID3D11RenderTargetView*	GetRenderTargetView() const   { return m_d3dRenderTargetView.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const   { return m_d3dDepthStencilView.Get(); }
        DXGI_FORMAT             GetBackBufferFormat() const   { return m_backBufferFormat; }
        DXGI_FORMAT             GetDepthBufferFormat() const  { return m_depthBufferFormat; }
        D3D11_VIEWPORT          GetScreenViewport() const     { return m_screenViewport; }
        UINT                    GetBackBufferCount() const    { return m_backBufferCount; }

        KennyKerr::Direct2D::DeviceContext&   GetD2DDeviceContext()       { return m_deviceContext; }
        KennyKerr::Direct2D::Factory1         GetD2DFactory() const       { return m_d2dFactory; }
        KennyKerr::DirectWrite::Factory1      GetDWriteFactory() const    { return m_dwriteFactory; }
        KennyKerr::Wic::Factory               GetWicFactory() const       { return m_wicFactory; }
		const DirectX::Keyboard::State&       GetKeyboardState() const    { return m_keyboardState; }
		const DirectX::Mouse::State&          GetMouseState() const       { return m_mouseState; }
		KennyKerr::DirectWrite::TextFormat&   GetTextFormat()             { return m_textFormat; }
		KennyKerr::Point2F                    GetMousePos() const         { return m_mousePos; }
		KennyKerr::Direct2D::SolidColorBrush& GetOrCreateColor(D2D1::ColorF::Enum solidColor);

		void SetWorldTranslation(D2D1::Matrix3x2F world) { m_world = world; }
		D2D1::Matrix3x2F GetWorldTranslation() const     { return m_world; }

        // Performance events
        void PIXBeginEvent(_In_z_ const wchar_t* name)
        {
            if (m_d3dAnnotation)
            {
                m_d3dAnnotation->BeginEvent(name);
            }
        }

        void PIXEndEvent()
        {
            if (m_d3dAnnotation)
            {
                m_d3dAnnotation->EndEvent();
            }
        }

        void PIXSetMarker(_In_z_ const wchar_t* name)
        {
            if (m_d3dAnnotation)
            {
                m_d3dAnnotation->SetMarker(name);
            }
        }

    private:
        void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);

        // Direct3D objects.
        Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;
        Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
        Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
        Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> m_d3dAnnotation;

		KennyKerr::Direct2D::DeviceContext              m_deviceContext;
		KennyKerr::Direct2D::Factory1                   m_d2dFactory;
		KennyKerr::DirectWrite::Factory1                m_dwriteFactory;
		KennyKerr::Wic::Factory                         m_wicFactory;
		DirectX::Keyboard                               m_keyboard;
		DirectX::Mouse                                  m_mouse;

        // Direct3D rendering objects. Required for 3D.
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_renderTarget;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
        D3D11_VIEWPORT                                  m_screenViewport;

        // Direct3D properties.
        DXGI_FORMAT                                     m_backBufferFormat;
        DXGI_FORMAT                                     m_depthBufferFormat;
        UINT                                            m_backBufferCount;
        D3D_FEATURE_LEVEL                               m_d3dMinFeatureLevel;

        // Cached device properties.
        HWND                                            m_window;
        D3D_FEATURE_LEVEL                               m_d3dFeatureLevel;
        RECT                                            m_outputSize;

        // The IDeviceNotify can be held directly as it owns the DeviceResources.
        IDeviceNotify*                                  m_deviceNotify;

		// color map
		std::unordered_map<D2D1::ColorF::Enum, KennyKerr::Direct2D::SolidColorBrush> m_colors;

		// text format
		KennyKerr::DirectWrite::TextFormat              m_textFormat;

		// input device status
		DirectX::Mouse::State                           m_mouseState;
		DirectX::Keyboard::State                        m_keyboardState;

		// world translation
		D2D1::Matrix3x2F                                m_world;

		// mouse position
		KennyKerr::Point2F                              m_mousePos;
    };
}