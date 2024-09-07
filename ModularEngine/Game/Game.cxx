module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

#if IN_RELEASE
#define SCREEN_WIDTH    static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN))
#else
#define SCREEN_WIDTH    800u
#define SCREEN_HEIGHT   600u
#endif // IN_RELEASE

module StarField;

import StarField.StarFactory;

namespace dx = DirectX;

namespace starfield
{
    Game::Game()
        :
        wnd_{ L"StarField", NAMESPACE_UTIL::ScreenSizeInfo{ SCREEN_WIDTH, SCREEN_HEIGHT } },
        gfx_{ wnd_.GetHwnd(), NAMESPACE_UTIL::ScreenSizeInfo{ wnd_.GetClientWidth<UINT>(), wnd_.GetClientHeight<UINT>() }}, // they are the same as SCREEN_ MACROS
        camera_{ gfx_ },
        camera_controller_{ camera_, wnd_.GetMouse(), wnd_.GetKeyboard() },
        drawables_{ StarFactory{}.GetStars() }
    {
         
    }

    auto Game::Go() -> int
    {
        std::optional<WPARAM> error_code;

        while (true)
        {
            error_code = NAMESPACE_WIN32::Window::ProcessMessages();

            if (error_code) [[unlikely]]
            {
                return static_cast<int>(*error_code);
            }

            if (wnd_.GetKeyboard().KeyIsPressed(VK_ESCAPE)) [[unlikely]]
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

    void Game::UpdateModel_() noexcept
    {
        const float deltaTime = timer_.Mark();

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