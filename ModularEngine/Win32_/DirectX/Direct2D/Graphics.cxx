module;

#include "../../FatWin32_.hpp"

#include <DirectXMath.h>

#include <d2d1.h>

#include <wrl.h>

#define SOFT_COLOR_EFFECT false

#pragma comment(lib, "d2d1")

module FatPound.Win32.D2D.Graphics;

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d2d
{
    Graphics::Graphics(HWND hWnd, const SizeInfo& dimensions)
        :
        width_(dimensions.width),
        height_(dimensions.height)
    {
        HRESULT hr;
        
        wrl::ComPtr<ID2D1Factory> pFactory = nullptr;

        hr = D2D1CreateFactory<ID2D1Factory>(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("A problem occured when creating the Factory!");
        }

        RECT rect = {};
        GetClientRect(hWnd, &rect);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
            &pRenderTarget_
        );

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("A problem occured when creating the HwndRenderTarget!");
        }
    }

    void Graphics::BeginFrame()
    {
        pRenderTarget_->BeginDraw();
    }
    void Graphics::EndFrame()
    {
        pRenderTarget_->EndDraw();
    }

    void Graphics::DrawLine(const D2D1_POINT_2F& p0, const D2D1_POINT_2F& p1, [[maybe_unused]] const D2D1_COLOR_F& color)
    {
        pRenderTarget_->DrawLine(p0, p1, pBrush_.Get());
    }
    void Graphics::DrawClosedPolyLine(const std::vector<dx::XMFLOAT2>& vertices, const D2D1_COLOR_F& color)
    {
        pRenderTarget_->CreateSolidColorBrush(color, &pBrush_);

        for (std::size_t i = 0u; i < vertices.size(); ++i)
        {
            const auto& current = vertices[i];
            const auto& next = vertices[(i + 1u) % vertices.size()];
            
            DrawLine(
                D2D1::Point2F(current.x, current.y),
                D2D1::Point2F(next.x, next.y),
                color
            );
        }
    }
    void Graphics::DrawClosedPolyLine(const std::vector<dx::XMFLOAT2>& vertices, const D2D1_COLOR_F& color, dx::XMMATRIX transform)
    {
        pRenderTarget_->CreateSolidColorBrush(color, &pBrush_);

        for (std::size_t i = 1u; i < vertices.size() + 1u; ++i)
        {
            const auto& vec0 = dx::XMVector2TransformCoord(dx::XMLoadFloat2(&vertices[i - 1u]), transform);
            const auto& vec1 = dx::XMVector2TransformCoord(dx::XMLoadFloat2(&vertices[i % vertices.size()]), transform);
        
            dx::XMFLOAT2 transformed0;
            dx::XMFLOAT2 transformed1;
            dx::XMStoreFloat2(&transformed0, vec0);
            dx::XMStoreFloat2(&transformed1, vec1);
        
            DrawLine(
                D2D1::Point2F(transformed0.x, transformed0.y),
                D2D1::Point2F(transformed1.x, transformed1.y),
                color
            );
        }
    }

    void Graphics::ClearScreen_(float r, float g, float b)
    {
        pRenderTarget_->Clear(D2D1::ColorF(r, g, b));
    }
}