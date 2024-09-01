module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.View.Camera;

import StarField.Entity.Drawable;

import FatPound;

import std;

export namespace starfield
{
    class Camera final
    {
    public:
        Camera(NAMESPACE_D2D::Graphics& gfx) noexcept;

        Camera() = delete;
        Camera(const Camera& src) = delete;
        Camera(Camera&& src) = delete;

        Camera& operator = (const Camera& src) = delete;
        Camera& operator = (Camera&& src) = delete;
        ~Camera() noexcept = default;


    public:
        auto GetPos() const noexcept -> DirectX::XMFLOAT2;

        auto GetViewportRect(const NAMESPACE_D2D::Graphics& gfx) const noexcept -> NAMESPACE_MATH::RectF;

        float GetAngle() const noexcept;
        float GetScale() const noexcept;

        void SetAngle(const float angle) noexcept;
        void SetScale(const float scale) noexcept;

        void MoveBy(const DirectX::XMFLOAT2& offset) noexcept;
        void MoveTo(const DirectX::XMFLOAT2& pos) noexcept;
        void Draw(entity::Drawable* drawable) const;


    protected:


    private:
        NAMESPACE_D2D::Graphics& gfx_;

        DirectX::XMFLOAT2 pos_ = { 0.0f, 0.0f };

        float angle_ = 0.0f;
        float scale_ = 1.0f;
    };
}