module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

module StarField.Entity.Star;

namespace dx = DirectX;

namespace starfield::entity
{
    Star::Star(const Descriptor& desc)
        :
        m_model_{ Star::Make(desc.radiuses.outer_radius, desc.radiuses.inner_radius, desc.flareCount) },
        m_desc_{ desc },
        m_faded_color_{ desc.color }
    {

    }

    auto Star::Make(const float outerRadius, const float innerRadius, const std::size_t flareCount) -> std::vector<dx::XMFLOAT2>
    {
        std::vector<dx::XMFLOAT2> star;

        const std::size_t vertexCount = flareCount * 2u;
        star.reserve(vertexCount);

        const float theta = 2.0f * dx::XM_PI / static_cast<float>(vertexCount);

        for (std::size_t i = 0; i < vertexCount; ++i)
        {
            const float& radius =
                i % 2 == 0
                ? outerRadius
                : innerRadius
                ;

            const float angle = static_cast<float>(i) * theta;

            const dx::XMVECTOR direction = dx::XMVectorSet(std::cos(angle), std::sin(angle), 0.0f, 0.0f);
            const dx::XMVECTOR point = dx::XMVectorScale(direction, radius);

            dx::XMFLOAT2 point2D;
            dx::XMStoreFloat2(&point2D, point);

            star.emplace_back(point2D);
        }

        return star;
    }

    auto Star::CheckCollision(const Star& star1, const Star& star2) noexcept -> bool
    {
        return star1.CollidesWith(star2);
    }

    auto Star::GetBoundingRect() const noexcept -> FATSPACE_MATH::RectF
    {
        return FATSPACE_MATH::RectF::FromCenter(m_desc_.position, m_desc_.radiuses.outer_radius, m_desc_.radiuses.outer_radius);
    }

    void Star::ApplyTransformation(const dx::XMMATRIX& transformer) noexcept
    {
        m_transformation_ *= transformer;
    }
    void Star::UpdateTo(const float total_time) noexcept
    {
        const float radiusOffset = m_desc_.radiusAmplitude * std::sin(m_desc_.radiusFrequency * total_time + m_desc_.radiusPhase);
        const float scale        = 1.0f + radiusOffset;
        const float angle        = m_desc_.rotationSpeed * total_time;

        const auto& translation = dx::XMMatrixTranslation(m_desc_.position.x, m_desc_.position.y, 0.0f);
        const auto& rotation    = dx::XMMatrixRotationZ(angle);
        const auto& scaling     = dx::XMMatrixScaling(scale, scale, 1.0f);

        m_transformation_ = rotation * translation * scaling;

        const int offset = static_cast<int>(127.0f * std::sin(m_desc_.colorFrequency * total_time + m_desc_.colorPhase)) + 128;

        m_faded_color_.r = static_cast<float>(std::min(static_cast<int>(m_desc_.color.r * 255) + offset, 255)) / 255.0f;
        m_faded_color_.g = static_cast<float>(std::min(static_cast<int>(m_desc_.color.g * 255) + offset, 255)) / 255.0f;
        m_faded_color_.b = static_cast<float>(std::min(static_cast<int>(m_desc_.color.b * 255) + offset, 255)) / 255.0f;
    }
    void Star::Draw(FATSPACE_D2D::Graphics& gfx) const noexcept
    {
        gfx.DrawClosedPolyLine(m_model_, GetFadedColor_(), m_transformation_);
    }

    auto Star::GetPos() const noexcept -> DirectX::XMFLOAT2
    {
        return m_desc_.position;
    }

    auto Star::GetMaxRadius() const noexcept -> float
    {
        return m_desc_.radiuses.outer_radius * (1.0f + m_desc_.radiusAmplitude);
    }

    auto Star::CollidesWith(const Star& star) const noexcept -> bool
    {
        return IsWithinArea(star.GetPos(), star.GetMaxRadius());
    }
    auto Star::IsWithinArea(const dx::XMFLOAT2& position, const float radius) const noexcept -> bool
    {
        const float distance = FATSPACE_MATH::GetDistanceBetweenXMF2(this->GetPos(), position);
        const float maxradsum = this->GetMaxRadius() + radius;

        return maxradsum > distance;
    }

    auto Star::GetFadedColor_() const noexcept -> D2D1_COLOR_F
    {
        return m_faded_color_;
    }
}