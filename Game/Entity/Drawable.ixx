module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

#include <DirectXMath.h>

export module StarField.Entity.Drawable;

import FatPound;

import std;

export namespace starfield::entity
{
    class Drawable
    {
    public:
        Drawable()                    = default;
        Drawable(const Drawable&)     = delete;
        Drawable(Drawable&&) noexcept = delete;

        auto operator = (const Drawable&)     -> Drawable& = delete;
        auto operator = (Drawable&&) noexcept -> Drawable& = delete;
        virtual ~Drawable() noexcept                       = default;


    public:
        virtual auto GetBoundingRect() const -> FATSPACE_MATH::RectF = 0;

        virtual void ApplyTransformation(const DirectX::XMMATRIX& transformer) = 0;
        virtual void UpdateTo(const float total_time) = 0;
        virtual void Draw(FATSPACE_D2D::Graphics& gfx) const = 0;


    protected:


    private:
    };
}

module : private;