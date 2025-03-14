module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

module StarField.View.Camera;

namespace dx = DirectX;

namespace starfield::view
{
    Camera::Camera(FATSPACE_D2D::Graphics& gfx) noexcept
        :
        m_gfx_(gfx)
    {

    }

    auto Camera::GetMatrix() const -> ::dx::XMMATRIX
    {
        return ::dx::XMMatrixTranslation(-m_pos_.x, -m_pos_.y, 0.0f) *
            ::dx::XMMatrixScaling(m_scale_, m_scale_, 1.0f) *
            ::dx::XMMatrixRotationZ(m_angle_) *
            ::dx::XMMatrixScaling(1.0f, -1.0f, 1.0f) *
            ::dx::XMMatrixTranslation(m_offset_.x, m_offset_.y, 0.0f);
    }

    auto Camera::GetPos() const noexcept -> ::dx::XMFLOAT2
    {
        return m_pos_;
    }

    auto Camera::GetViewportRect(const FATSPACE_D2D::Graphics& gfx) const noexcept -> FATSPACE_MATH::Rect<float>
    {
        const auto zoom = 1.0f / m_scale_;

        const auto diagonal = ::std::sqrt(
            FATSPACE_MATH::Square(m_gfx_.GetWidth<float>()  / 2.0f * zoom)
            +
            FATSPACE_MATH::Square(m_gfx_.GetHeight<float>() / 2.0f * zoom)
        );

        return FATSPACE_MATH::Rect<float>::FromCenter(
            m_pos_.x,
            m_pos_.y,
            diagonal,
            diagonal
        );
    }

    auto Camera::GetAngle() const noexcept -> float
    {
        return m_angle_;
    }
    auto Camera::GetScale() const noexcept -> float
    {
        return m_scale_;
    }

    void Camera::SetAngle(const float angle) noexcept
    {
        m_angle_ = angle;
    }
    void Camera::SetScale(const float scale) noexcept
    {
        m_scale_ = scale;
    }
    void Camera::MoveBy(const ::dx::XMFLOAT2& offset) noexcept
    {
        m_pos_.x += offset.x;
        m_pos_.y += offset.y;
    }
    void Camera::MoveTo(const ::dx::XMFLOAT2& pos) noexcept
    {
        m_pos_ = pos;
    }

}