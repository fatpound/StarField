module;

#include <_macros/Namespaces.hxx>

export module StarField;

import <d2d1.h>;
import <DirectXMath.h>;

import StarField.Entity;
import StarField.View;
import StarField.StarFactory;

import FatPound;

import std;

#ifdef IN_RELEASE
    #define ScreenWidth  static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN))
    #define ScreenHeight static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN))
#else
    static constexpr UINT ScreenWidth  = 800U;
    static constexpr UINT ScreenHeight = 600U;
#endif

namespace dx = DirectX;

export namespace starfield
{
    class Game final
    {
        using Settings = StarFactory<>::Settings;

    public:
        explicit Game(const Settings& settings = {})
            :
            m_wnd_{ std::make_shared<FATSPACE_WIN32::WndClassEx>(L"fat->pound WindowClassEx: " + std::to_wstring(s_game_id_++)), L"StarField " + std::to_wstring(s_game_id_), FATSPACE_UTILITY::SizePack{ ScreenWidth, ScreenHeight } },
            m_gfx_{ m_wnd_.GetHandle(), FATSPACE_UTILITY::SizePack{ m_wnd_.GetClientWidth<UINT>(), m_wnd_.GetClientHeight<UINT>() } },
            m_camera_{ m_gfx_ },
            m_camera_controller_{ m_camera_, *m_wnd_.m_pMouse, *m_wnd_.m_pKeyboard },
            m_drawables_{ StarFactory<>{settings}.GetStars() },
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

        explicit Game(const Game&)     = delete;
        explicit Game(Game&&) noexcept = delete;

        auto operator = (const Game& src)     -> Game& = delete;
        auto operator = (Game&& src) noexcept -> Game& = delete;
        ~Game() noexcept                               = default;


    public:
        auto IsRunning() const -> bool
        {
            return not IsOver();
        }
        auto IsOver()    const -> bool
        {
            return m_wnd_.IsClosing();
        }


    protected:


    private:
        void Go_()
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
        void UpdateModel_() noexcept
        {
            m_timer_.Stop();
            const auto& deltaTime = m_timer_.GetElapsed_s();
            m_timer_.Start();

            m_total_time_ += deltaTime;

            m_camera_controller_.Update(deltaTime);
        }
        void DoFrame_()
        {
            const auto& matrix = m_camera_.GetMatrix();
            const auto& viewport = m_camera_.GetViewportRect();

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


    private:
        inline static std::size_t s_game_id_{};


    private:
        FATSPACE_WIN32::WindowEx m_wnd_;
        FATSPACE_D2D::Graphics m_gfx_;

        FATSPACE_UTILITY::Timer m_timer_;

        view::Camera m_camera_;
        view::CameraController m_camera_controller_;

        std::vector<std::unique_ptr<entity::Star>> m_drawables_;

        float m_total_time_{};

        std::jthread m_game_loop_;
    };
}
