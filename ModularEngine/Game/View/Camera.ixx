module;

#include "../../Win32_/FatWin32_.hpp"

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
        Camera(NAMESPACE_D2D::Graphics& gfx);


    public:
        auto GetPos() const->DirectX::XMFLOAT2;

        auto GetViewportRect(NAMESPACE_D2D::Graphics& gfx) const->NAMESPACE_MATH::RectF;

        float GetAngle() const;
        float GetScale() const;

        void SetAngle(const float angle);
        void SetScale(const float scale);

        void MoveBy(const DirectX::XMFLOAT2& offset);
        void MoveTo(const DirectX::XMFLOAT2& pos);
        void Draw(entity::Drawable* drawable) const;


    protected:


    private:
        NAMESPACE_D2D::Graphics& gfx_;

        DirectX::XMFLOAT2 pos_ = { 0.0f, 0.0f };

        float angle_ = 0.0f;
        float scale_ = 1.0f;
    };
}