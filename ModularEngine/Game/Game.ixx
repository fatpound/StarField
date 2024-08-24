module;

#include "../Win32_/FatWin32_Namespaces.hpp"

#include <DirectXMath.h>

export module StarField;

import StarField.Entity;
import StarField.View;

import FatPound;

import std;

export namespace starfield
{
    class Game final
    {
    public:
        Game();
        Game(const Game& src) = delete;
        Game& operator = (const Game& src) = delete;

        Game(Game&& src) = delete;
        Game& operator = (Game&& src) = delete;
        ~Game() noexcept = default;


    public:
        int Go();


    protected:


    private:
        void UpdateModel_();
        void DoFrame_();


    private:
        NAMESPACE_WIN32::Window wnd_;

        NAMESPACE_D2D::Graphics gfx_;

        NAMESPACE_UTIL::AutoTimer timer_;

        Camera camera_;
        CameraController camera_controller_;

        std::vector<std::unique_ptr<entity::Star>> drawables_;

        float totalTime_ = 0.0f;
    };
}