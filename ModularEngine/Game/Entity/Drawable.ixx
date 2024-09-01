module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

export module StarField.Entity.Drawable;

import FatPound;

import std;

export namespace starfield::entity
{
    class Drawable
    {
    public:
        Drawable() = default;
        Drawable(const Drawable& src) = delete;
        Drawable(Drawable&& src) = delete;

        Drawable& operator = (const Drawable& src) = delete;
        Drawable& operator = (Drawable&& src) = delete;
        virtual ~Drawable() noexcept = default;


    public:
        virtual auto GetBoundingRect() const -> NAMESPACE_MATH::RectF = 0;

        virtual void ApplyTransformation(const DirectX::XMMATRIX& transformer) = 0;
        virtual void UpdateTo(float total_time) = 0;
        virtual void Draw(NAMESPACE_D2D::Graphics& gfx) const = 0;


    protected:


    private:
    };
}