module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

#include <DirectXMath.h>

export module StarField.View.CameraController;

import <d2d1.h>;

import StarField.View.Camera;

import FatPound;

import std;

export namespace starfield::view
{
    class CameraController final
    {
    public:
        explicit CameraController(Camera& camera, FATSPACE_IO::Mouse& mouse, const FATSPACE_IO::Keyboard& kbd) noexcept;

        explicit CameraController()                            = delete;
        explicit CameraController(const CameraController&)     = delete;
        explicit CameraController(CameraController&&) noexcept = delete;

        auto operator = (const CameraController&)     -> CameraController& = delete;
        auto operator = (CameraController&&) noexcept -> CameraController& = delete;
        ~CameraController() noexcept                                       = default;


    public:
        void Update(float deltaTime) noexcept;


    protected:


    private:
        Camera& m_camera_;

        FATSPACE_IO::Mouse& m_mouse_;
        const FATSPACE_IO::Keyboard& m_kbd_;

        ::DirectX::XMFLOAT2 m_last_pos_{ 0.0f, 0.0f };

        bool engaged_{};

        static constexpr auto zoomFactor_ = 1.05f;
        static constexpr auto rotationSpeed_ = ::DirectX::XM_PI / 6.0f;
    };
}

module : private;
