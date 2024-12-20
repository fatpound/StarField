module;

#include <FatNamespaces.hpp>

#include <FatWin32.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.Entity.Star;

import StarField.Entity.Drawable;

import FatPound;

import std;

export namespace starfield::entity
{
    class Star final : public Drawable
    {
    public:
        struct RadiusPack final
        {
            float outer_radius;
            float inner_radius;
        };
        struct Descriptor final
        {
            ::DirectX::XMFLOAT2 position;

            RadiusPack radiuses;

            D2D1_COLOR_F color;

            std::size_t flareCount;

            float colorFrequency;
            float colorPhase;

            float radiusAmplitude;
            float radiusFrequency;
            float radiusPhase;

            float rotationSpeed;
        };


    public:
        Star(const Descriptor& desc);

        Star() = delete;
        Star(const Star& src) = delete;
        Star(Star&& src) = delete;

        auto operator = (const Star& src) -> Star& = delete;
        auto operator = (Star&& src) -> Star& = delete;
        virtual ~Star() noexcept = default;


    public:
        static auto Make(const float outerRadius, const float innerRadius, const std::size_t flareCount = 5u) -> std::vector<::DirectX::XMFLOAT2>;

        static auto CheckCollision(const Star& star1, const Star& star2) noexcept -> bool;


    public:
        virtual auto GetBoundingRect() const noexcept -> FATSPACE_MATH::RectF override final;

        virtual void ApplyTransformation(const ::DirectX::XMMATRIX& transformer) noexcept override final;
        virtual void UpdateTo(const float total_time) noexcept override final;
        virtual void Draw(FATSPACE_D2D::Graphics& gfx) const noexcept override final;


    public:
        auto GetPos() const noexcept -> ::DirectX::XMFLOAT2;

        auto GetMaxRadius() const noexcept -> float;

        auto CollidesWith(const Star& star) const noexcept -> bool;
        auto IsWithinArea(const ::DirectX::XMFLOAT2& position, const float radius) const noexcept -> bool;


    protected:


    private:
        auto GetFadedColor_() const noexcept -> D2D1_COLOR_F;


    private:
        std::vector<::DirectX::XMFLOAT2> m_model_;

        ::DirectX::XMMATRIX m_transformation_;

        Descriptor m_desc_;

        D2D1_COLOR_F m_faded_color_;
    };
}

module : private;