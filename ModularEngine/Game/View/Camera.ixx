module;

#include <FatWin32.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.View.Camera;

import FatPound;

import std;

export namespace starfield
{
    class Camera final
    {
    public:
        explicit Camera(FATSPACE_D2D::Graphics& gfx) noexcept;

        explicit Camera() = delete;
        explicit Camera(const Camera& src) = delete;
        explicit Camera(Camera&& src) = delete;

        auto operator = (const Camera& src) -> Camera& = delete;
        auto operator = (Camera&& src)      -> Camera& = delete;
        ~Camera() noexcept = default;


    public:
        auto GetMatrix() const -> ::DirectX::XMMATRIX;

        auto GetPos() const noexcept -> ::DirectX::XMFLOAT2;

        auto GetViewportRect(const FATSPACE_D2D::Graphics& gfx) const noexcept -> FATSPACE_MATH::RectF;

        auto GetAngle() const noexcept -> float;
        auto GetScale() const noexcept -> float;

        void SetAngle(const float angle) noexcept;
        void SetScale(const float scale) noexcept;

        void MoveBy(const ::DirectX::XMFLOAT2& offset) noexcept;
        void MoveTo(const ::DirectX::XMFLOAT2& pos) noexcept;



    protected:


    private:
        FATSPACE_D2D::Graphics& m_gfx_;

        ::DirectX::XMFLOAT2 m_pos_{ 0.0f, 0.0f };

        const ::DirectX::XMFLOAT2 m_offset_
        {
            static_cast<float>(m_gfx_.mc_width)  / 2.0f,
            static_cast<float>(m_gfx_.mc_height) / 2.0f
        };

        float m_angle_ = 0.0f;
        float m_scale_ = 1.0f;
    };
}