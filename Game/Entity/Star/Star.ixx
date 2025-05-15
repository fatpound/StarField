module;

#include <FatNamespaces.hpp>

#include <d2d1.h>

export module StarField.Entity.Star;

import <DirectXMath.h>;

import StarField.Entity.Drawable;

import FatPound;

import std;

namespace dx = DirectX;

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
            dx::XMFLOAT2 position;

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
        Star(const Descriptor& desc)
            :
            m_model_{ Star::Make(desc.radiuses.outer_radius, desc.radiuses.inner_radius, desc.flareCount) },
            m_desc_{ desc },
            m_faded_color_{ desc.color }
        {

        }

        Star()                = delete;
        Star(const Star&)     = delete;
        Star(Star&&) noexcept = delete;

        auto operator = (const Star&)     -> Star& = delete;
        auto operator = (Star&&) noexcept -> Star& = delete;
        virtual ~Star() noexcept                   = default;


    public:
        static auto Make(const float outerRadius, const float innerRadius, const std::size_t& flareCount = 5U) -> std::vector<dx::XMFLOAT2>
        {
            std::vector<dx::XMFLOAT2> star;

            const std::size_t vertexCount = flareCount * 2u;
            star.reserve(vertexCount);

            const float theta = 2.0f * dx::XM_PI / static_cast<float>(vertexCount);

            for (std::size_t i{}; i < vertexCount; ++i)
            {
                const auto& radius = (i % 2 == 0)
                    ? outerRadius
                    : innerRadius
                    ;

                const float angle = static_cast<float>(i) * theta;

                const auto& direction = dx::XMVectorSet(std::cos(angle), std::sin(angle), 0.0F, 0.0F);
                const auto& point     = dx::XMVectorScale(direction, radius);

                dx::XMFLOAT2 point2D;
                dx::XMStoreFloat2(&point2D, point);

                star.emplace_back(point2D);
            }

            return star;
        }

        static auto CheckCollision(const Star& star1, const Star& star2) noexcept -> bool
        {
            return star1.CollidesWith(star2);
        }


    public:
        virtual auto GetBoundingRect() const noexcept -> FATSPACE_MATH::geometry::Rect<float> override final
        {
            return FATSPACE_MATH::geometry::Rect<float>::FromCenter(
                m_desc_.position.x,
                m_desc_.position.y,
                m_desc_.radiuses.outer_radius,
                m_desc_.radiuses.outer_radius
            );
        }

        virtual void ApplyTransformation(const dx::XMMATRIX& transformer) noexcept override final
        {
            m_transformation_ *= transformer;
        }
        virtual void UpdateTo(const float total_time) noexcept override final
        {
            const float radiusOffset = m_desc_.radiusAmplitude * std::sin(m_desc_.radiusFrequency * total_time + m_desc_.radiusPhase);
            const float scale = 1.0F + radiusOffset;
            const float angle = m_desc_.rotationSpeed * total_time;

            const auto& translation = dx::XMMatrixTranslation(m_desc_.position.x, m_desc_.position.y, 0.0F);
            const auto& rotation    = dx::XMMatrixRotationZ(angle);
            const auto& scaling     = dx::XMMatrixScaling(scale, scale, 1.0F);

            m_transformation_ = rotation * translation * scaling;

            const int offset = static_cast<int>(127.0F * std::sin(m_desc_.colorFrequency * total_time + m_desc_.colorPhase)) + 128;

            m_faded_color_.r = static_cast<float>(std::min(static_cast<int>(m_desc_.color.r * 255) + offset, 255)) / 255.0F;
            m_faded_color_.g = static_cast<float>(std::min(static_cast<int>(m_desc_.color.g * 255) + offset, 255)) / 255.0F;
            m_faded_color_.b = static_cast<float>(std::min(static_cast<int>(m_desc_.color.b * 255) + offset, 255)) / 255.0F;
        }
        virtual void Draw(FATSPACE_D2D::Graphics& gfx) const noexcept override final
        {
            gfx.DrawClosedPolyLine(m_model_, GetFadedColor_(), m_transformation_);
        }


    public:
        auto GetPos() const noexcept -> dx::XMFLOAT2
        {
            return m_desc_.position;
        }

        auto GetMaxRadius() const noexcept -> float
        {
            return m_desc_.radiuses.outer_radius * (1.0F + m_desc_.radiusAmplitude);
        }

        auto CollidesWith(const Star& star) const noexcept -> bool
        {
            return IsWithinArea(star.GetPos(), star.GetMaxRadius());
        }
        auto IsWithinArea(const dx::XMFLOAT2& position, const float radius) const noexcept -> bool
        {
            using FATSPACE_MATH::operator -;

            const auto& distance  = this->GetPos() - position;
            const auto& maxradsum = this->GetMaxRadius() + radius;

            return maxradsum > distance;
        }


    protected:


    private:
        auto GetFadedColor_() const noexcept -> D2D1_COLOR_F
        {
            return m_faded_color_;
        }


    private:
        std::vector<dx::XMFLOAT2> m_model_;

        dx::XMMATRIX m_transformation_;

        Descriptor m_desc_;

        D2D1_COLOR_F m_faded_color_;
    };
}
