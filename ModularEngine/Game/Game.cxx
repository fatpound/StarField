module;

#include "../Win32_/FatWin32_.hpp"

#include <DirectXMath.h>

#include <d2d1.h>

#if IN_RELEASE
#define CLIENT_WIDTH    GetSystemMetrics(SM_CXSCREEN)
#define CLIENT_HEIGHT   GetSystemMetrics(SM_CYSCREEN)
#else
#define CLIENT_WIDTH    800
#define CLIENT_HEIGHT   600
#endif // IN_RELEASE

module StarField;

import StarField.StarFactory;

namespace dx = DirectX;

namespace starfield
{
    Game::Game()
        :
        wnd_{ L"StarField", NAMESPACE_WIN32::Window::ClientSizeInfo{ CLIENT_WIDTH, CLIENT_HEIGHT } },
        gfx_{ wnd_.GetHwnd(), NAMESPACE_D2D::Graphics::SizeInfo{ wnd_.GetClientWidth<int>(), wnd_.GetClientHeight<int>() }}, // they are the same as SCREEN_ MACROS
        camera_{ gfx_ },
        camera_controller_{ camera_, wnd_.mouse, wnd_.kbd },
        drawables_{ StarFactory{}.GetStars() }
    {
         
    }

    int Game::Go()
    {
        std::optional<WPARAM> error_code;

        while (true)
        {
            error_code = NAMESPACE_WIN32::Window::ProcessMessages();

            if (error_code) [[unlikely]]
            {
                return static_cast<int>(*error_code);
            }

            if (wnd_.kbd.KeyIsPressed(VK_ESCAPE)) [[unlikely]]
            {
                wnd_.Kill();

                return 0;
            }

            gfx_.BeginFrame();
            UpdateModel_();
            DoFrame_();
            gfx_.EndFrame();
        }
    }

    void Game::UpdateModel_()
    {
        float deltaTime = timer_.Mark();

        totalTime_ += deltaTime;

        camera_controller_.Update(deltaTime);
    }

    void Game::DoFrame_()
    {
        const auto& viewport = camera_.GetViewportRect(gfx_);

        for (auto& drawable : drawables_)
        {
            if (drawable->GetBoundingRect().IsOverlappingWith(viewport))
            {
                drawable->UpdateTo(totalTime_);

                camera_.Draw(drawable.get());
            }
        }
    }
}