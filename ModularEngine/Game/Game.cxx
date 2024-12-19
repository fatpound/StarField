module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

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
        m_wnd_{ std::make_shared<FATSPACE_WIN32::WndClassEx>(L"fat->pound WindowClassEx: " + std::to_wstring(s_game_id_++)), L"StarField " + std::to_wstring(s_game_id_), FATSPACE_UTIL::ScreenSizeInfo{ SCREEN_WIDTH, SCREEN_HEIGHT } },
        m_gfx_{ m_wnd_.GetHandle(), FATSPACE_UTIL::ScreenSizeInfo{ m_wnd_.GetClientWidth<UINT>(), m_wnd_.GetClientHeight<UINT>() }},
        m_camera_{ m_gfx_ },
        m_camera_controller_{ m_camera_, *m_wnd_.m_pMouse, *m_wnd_.m_pKeyboard },
        m_drawables_{ StarFactory<>{}.GetStars()},
        ////////////////////////////////
#pragma region (gameloop w/o C4355)
#pragma warning (push)
#pragma warning (disable : 4355)
        m_game_loop_{ &Game::Go_, this }
#pragma warning (pop)
#pragma endregion
        ////////////////////////////////
    {
         
    }

    auto Game::IsRunning() const -> bool
    {
        return not IsOver();
    }
    auto Game::IsOver() const -> bool
    {
        return m_wnd_.IsClosing();
    }

    void Game::Go_()
    {
        m_timer_.Start();

        while (IsRunning())
        {
            m_gfx_.BeginFrame<false>();
            UpdateModel_();
            DoFrame_();
            m_gfx_.EndFrame();
        }
    }
    void Game::UpdateModel_() noexcept
    {
        m_timer_.Stop();
        const auto& deltaTime = m_timer_.GetElapsed_s();
        m_timer_.Start();

        m_total_time_ += deltaTime;

        m_camera_controller_.Update(deltaTime);
    }
    void Game::DoFrame_()
    {
        const auto& matrix   = m_camera_.GetMatrix();
        const auto& viewport = m_camera_.GetViewportRect(m_gfx_);

        for (auto& drawable : m_drawables_)
        {
            if (drawable->GetBoundingRect().IsOverlappingWith(viewport))
            {
                drawable->UpdateTo(m_total_time_);
                drawable->ApplyTransformation(matrix);
                drawable->Draw(m_gfx_);
            }
        }
    }
}