module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

module StarField.View.Camera;

namespace dx = DirectX;

namespace starfield
{
    Camera::Camera(NAMESPACE_D2D::Graphics& gfx) noexcept
        :
        gfx_(gfx)
    {

    }

    auto Camera::GetPos() const noexcept -> dx::XMFLOAT2
    {
        return pos_;
    }

    auto Camera::GetViewportRect(const NAMESPACE_D2D::Graphics& gfx) const noexcept -> NAMESPACE_MATH::RectF
    {
        const float zoom = 1.0f / scale_;

        const float diagonal = std::sqrt(
            NAMESPACE_MATH::Square(static_cast<float>(gfx.width_)  / 2.0f * zoom)
            +
            NAMESPACE_MATH::Square(static_cast<float>(gfx.height_) / 2.0f * zoom)
        );

        return NAMESPACE_MATH::RectF::FromCenter(
            pos_,
            diagonal,
            diagonal
        );
    }

    float Camera::GetAngle() const noexcept
    {
        return angle_;
    }
    float Camera::GetScale() const noexcept
    {
        return scale_;
    }

    void Camera::SetAngle(const float angle) noexcept
    {
        angle_ = angle;
    }
    void Camera::SetScale(const float scale) noexcept
    {
        scale_ = scale;
    }
    void Camera::MoveBy(const dx::XMFLOAT2& offset) noexcept
    {
        pos_.x += offset.x;
        pos_.y += offset.y;
    }
    void Camera::MoveTo(const dx::XMFLOAT2& pos) noexcept
    {
        pos_ = pos;
    }

    void Camera::Draw(entity::Drawable* drawable) const
    {
        const dx::XMFLOAT2 offset =
        {
            static_cast<float>(gfx_.width_)  / 2.0f,
            static_cast<float>(gfx_.height_) / 2.0f
        };

        const auto& transform =
            dx::XMMatrixTranslation(-pos_.x, -pos_.y, 0.0f) *
            dx::XMMatrixScaling(scale_, scale_, 1.0f) *
            dx::XMMatrixRotationZ(angle_) *
            dx::XMMatrixScaling(1.0f, -1.0f, 1.0f) *
            dx::XMMatrixTranslation(offset.x, offset.y, 0.0f);

        drawable->ApplyTransformation(transform);
        drawable->Draw(gfx_);
    }
}