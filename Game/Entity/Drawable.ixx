module;

#include <FatNamespaces.hpp>

export module StarField.Entity.Drawable;

import <DirectXMath.h>;

import FatPound;

import std;

namespace dx = DirectX;

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
        virtual auto GetBoundingRect() const -> FATSPACE_MATH::geometry::Rect<float> = 0;

        virtual void ApplyTransformation(const dx::XMMATRIX& transformer) = 0;
        virtual void UpdateTo(const float total_time) = 0;
        virtual void Draw(FATSPACE_D2D::Graphics& gfx) const = 0;


    protected:


    private:
    };
}

module : private;
