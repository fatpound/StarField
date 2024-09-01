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
        model_{ Star::Make(desc.radiuses.outer_radius, desc.radiuses.inner_radius, desc.flareCount) },
        desc_{ desc },
        faded_color_{ desc.color }
    {

    }

    auto Star::Make(float outerRadius, float innerRadius, std::size_t nFlares) -> std::vector<dx::XMFLOAT2>
    {
        std::vector<dx::XMFLOAT2> star;

        const std::size_t vertexCount = nFlares * 2;
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

    bool Star::CheckCollision(const Star& star1, const Star& star2) noexcept
    {
        return star1.CollidesWith(star2);
    }

    auto Star::GetBoundingRect() const noexcept -> NAMESPACE_MATH::RectF
    {
        return NAMESPACE_MATH::RectF::FromCenter(desc_.position, desc_.radiuses.outer_radius, desc_.radiuses.outer_radius);
    }

    void Star::ApplyTransformation(const dx::XMMATRIX& transformer) noexcept
    {
        transformation_ *= transformer;
    }
    void Star::UpdateTo(float total_time) noexcept
    {
        const float radiusOffset = desc_.radiusAmplitude * std::sin(desc_.radiusFrequency * total_time + desc_.radiusPhase);
        const float scale = 1.0f + radiusOffset;
        const float angle = desc_.rotationSpeed * total_time;

        const auto& translation = dx::XMMatrixTranslation(desc_.position.x, desc_.position.y, 0.0f);
        const auto& rotation    = dx::XMMatrixRotationZ(angle);
        const auto& scaling     = dx::XMMatrixScaling(scale, scale, 1.0f);

        transformation_ = rotation * translation * scaling;

        const int offset = static_cast<int>(127.0f * std::sin(desc_.colorFrequency * total_time + desc_.colorPhase)) + 128;

        faded_color_.r = static_cast<float>(std::min(static_cast<int>(desc_.color.r * 255) + offset, 255)) / 255.0f;
        faded_color_.g = static_cast<float>(std::min(static_cast<int>(desc_.color.g * 255) + offset, 255)) / 255.0f;
        faded_color_.b = static_cast<float>(std::min(static_cast<int>(desc_.color.b * 255) + offset, 255)) / 255.0f;
    }
    void Star::Draw(NAMESPACE_D2D::Graphics& gfx) const noexcept
    {
        gfx.DrawClosedPolyLine(model_, GetFadedColor_(), transformation_);
    }

    auto Star::GetPos() const noexcept -> DirectX::XMFLOAT2
    {
        return desc_.position;
    }

    float Star::GetMaxRadius() const noexcept
    {
        return desc_.radiuses.outer_radius * (1.0f + desc_.radiusAmplitude);
    }

    bool Star::CollidesWith(const Star& star) const noexcept
    {
        return IsWithinArea(star.GetPos(), star.GetMaxRadius());
    }
    bool Star::IsWithinArea(const dx::XMFLOAT2& position, float radius) const noexcept
    {
        const float distance = NAMESPACE_MATH::GetDistanceBetweenXMF2(this->GetPos(), position);
        const float maxradsum = this->GetMaxRadius() + radius;

        return maxradsum > distance;
    }

    auto Star::GetFadedColor_() const noexcept -> D2D1_COLOR_F
    {
        return faded_color_;
    }
}