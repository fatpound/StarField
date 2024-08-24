module;

#include <FatWin32_.hpp>

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
            DirectX::XMFLOAT2 position;

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


    public:
        static auto Make(float outerRadius, float innerRadius, std::size_t nFlares = 5u) -> std::vector<DirectX::XMFLOAT2>;

        static bool CheckCollision(const Star& star1, const Star& star2) noexcept;


    public:
        virtual auto GetBoundingRect() const -> NAMESPACE_MATH::RectF override final;

        virtual void ApplyTransformation(const DirectX::XMMATRIX& transformer) override final;
        virtual void UpdateTo(float total_time) override final;
        virtual void Draw(NAMESPACE_D2D::Graphics& gfx) const override final;


    public:
        auto GetPos() const -> DirectX::XMFLOAT2;

        float GetMaxRadius() const;

        bool CollidesWith(const Star& star) const noexcept;
        bool IsWithinArea(const DirectX::XMFLOAT2& position, const float& radius) const noexcept;


    protected:


    private:
        auto GetFadedColor_() const noexcept -> D2D1_COLOR_F;


    private:
        std::vector<DirectX::XMFLOAT2> model_;

        DirectX::XMMATRIX transformation_;

        Descriptor desc_;

        D2D1_COLOR_F faded_color_;
    };
}