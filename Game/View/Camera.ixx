module;

#include <FatNamespaces.hxx>

#include <d2d1.h>

export module StarField.View.Camera;

import <DirectXMath.h>;

import FatPound;

import std;

namespace dx = DirectX;

export namespace starfield::view
{
    class Camera final
    {
    public:
        explicit Camera(FATSPACE_D2D::Graphics& gfx) noexcept
            :
            m_gfx_(gfx)
        {

        }

        explicit Camera()                  = delete;
        explicit Camera(const Camera&)     = delete;
        explicit Camera(Camera&&) noexcept = delete;

        auto operator = (const Camera&)     -> Camera& = delete;
        auto operator = (Camera&&) noexcept -> Camera& = delete;
        ~Camera() noexcept                             = default;


    public:
        auto GetMatrix() const -> dx::XMMATRIX
        {
            return dx::XMMatrixTranslation(-m_pos_.x, -m_pos_.y, 0.0f) *
                dx::XMMatrixScaling(m_scale_, m_scale_, 1.0f) *
                dx::XMMatrixRotationZ(m_angle_) *
                dx::XMMatrixScaling(1.0f, -1.0f, 1.0f) *
                dx::XMMatrixTranslation(m_offset_.x, m_offset_.y, 0.0f);
        }

        auto GetPos() const noexcept -> dx::XMFLOAT2
        {
            return m_pos_;
        }

        auto GetViewportRect(const FATSPACE_D2D::Graphics& gfx) const noexcept -> FATSPACE_MATH::geometry::Rect<float>
        {
            const auto& zoom = 1.0f / m_scale_;

            const auto& diagonal = std::sqrt(
                FATSPACE_MATH::Square(m_gfx_.GetWidth<float>()  / 2.0f * zoom)
                +
                FATSPACE_MATH::Square(m_gfx_.GetHeight<float>() / 2.0f * zoom)
            );

            return FATSPACE_MATH::geometry::Rect<float>::FromCenter(
                m_pos_.x,
                m_pos_.y,
                diagonal,
                diagonal
            );
        }

        auto GetAngle() const noexcept -> float
        {
            return m_angle_;
        }
        auto GetScale() const noexcept -> float
        {
            return m_scale_;
        }

        void SetAngle(const float angle) noexcept
        {
            m_angle_ = angle;
        }
        void SetScale(const float scale) noexcept
        {
            m_scale_ = scale;
        }

        void MoveBy(const dx::XMFLOAT2& offset) noexcept
        {
            m_pos_.x += offset.x;
            m_pos_.y += offset.y;
        }
        void MoveTo(const dx::XMFLOAT2& pos) noexcept
        {
            m_pos_ = pos;
        }



    protected:


    private:
        FATSPACE_D2D::Graphics& m_gfx_;

        dx::XMFLOAT2 m_pos_{ 0.0f, 0.0f };

        const dx::XMFLOAT2 m_offset_
        {
            m_gfx_.GetWidth<float>()  / 2.0f,
            m_gfx_.GetHeight<float>() / 2.0f
        };

        float m_angle_ = 0.0f;
        float m_scale_ = 1.0f;
    };
}
