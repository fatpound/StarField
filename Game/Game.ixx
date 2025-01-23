module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

#include <d2d1.h>

#include <DirectXMath.h>

export module StarField;

import StarField.Entity;
import StarField.View;
import StarField.StarFactory;

import FatPound;

import std;

export namespace starfield
{
    class Game final
    {
        using Settings = StarFactory<>::Settings;

    public:
        explicit Game(const Settings& settings = {});

        explicit Game(const Game& src) = delete;
        explicit Game(Game&& src) = delete;

        auto operator = (const Game& src) -> Game& = delete;
        auto operator = (Game&& src)      -> Game& = delete;
        ~Game() noexcept = default;


    public:
        auto IsRunning() const -> bool;
        auto IsOver()    const -> bool;


    protected:


    private:
        void Go_();
        void UpdateModel_() noexcept;
        void DoFrame_();


    private:
        inline static std::size_t s_game_id_{};


    private:
        FATSPACE_WIN32::WindowEx m_wnd_;
        FATSPACE_D2D::Graphics m_gfx_;

        FATSPACE_UTIL::Timer m_timer_;

        view::Camera m_camera_;
        view::CameraController m_camera_controller_;

        std::vector<std::unique_ptr<entity::Star>> m_drawables_;

        float m_total_time_{};

        std::jthread m_game_loop_;
    };
}

module : private;