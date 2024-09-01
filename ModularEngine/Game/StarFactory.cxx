module;

#include <FatWin32_Settings.hpp>
#include <FatWin32_Namespaces.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

#include <algorithm>

module StarField.StarFactory;

namespace dx = DirectX;

namespace starfield
{
    StarFactory::StarFactory()
    {
        stars_.reserve(Settings_::starCount_);

        while (stars_.size() < Settings_::starCount_)
        {
            stars_.push_back(GenerateStar_());
        }
    }

    auto StarFactory::GetStars() && noexcept -> std::vector<std::unique_ptr<entity::Star>>&&
    {
        return std::move(stars_);
    }

    auto StarFactory::GenerateStar_() -> std::unique_ptr<entity::Star>
    {
    generation:

        const auto& starPosition = dx::XMFLOAT2{ dist_x_position_(rng_), dist_y_position_(rng_) };
        const auto& radius = std::clamp(dist_radius_(rng_), Settings_::minStarRadius_, Settings_::maxStarRadius_);
        const auto& radAmplitude = std::clamp(dist_radius_amplitude_(rng_), Settings_::minRadiusAmplitude_, Settings_::maxRadiusAmplitude_);
        const auto& maxRadius = radius * (1.0f + radAmplitude);

        if (std::ranges::any_of(stars_, [&](const auto& pstar) noexcept -> bool { return pstar->IsWithinArea(starPosition, maxRadius); }))
        {
            goto generation;
        }

        entity::Star::Descriptor desc = {};

        desc.position = starPosition;
        desc.radiuses.outer_radius = radius;
        desc.radiuses.inner_radius = radius * std::clamp(dist_radius_ratio_(rng_), Settings_::minStarInnerRatio_, Settings_::maxStarInnerRatio_);
        desc.flareCount = std::clamp(static_cast<std::size_t>(dist_flare_count_(rng_)), Settings_::minFlareCount_, Settings_::maxFlareCount_);
        desc.color = colors[dist_color_(rng_)];
        desc.colorFrequency = std::clamp(dist_color_frequency_(rng_), Settings_::minColorFrequency_, Settings_::maxColorFrequency_);
        desc.colorPhase = dist_phase_(rng_);
        desc.radiusFrequency = dist_radius_frequency_(rng_);
        desc.radiusPhase = dist_phase_(rng_);
        desc.radiusAmplitude = radAmplitude;
        desc.rotationSpeed = dist_rotation_speed_(rng_);

        return std::make_unique<entity::Star>(desc);
    }
}