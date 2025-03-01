module;

#include <FatWin32.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.StarFactory;

import StarField.Entity.Star;

import std;

namespace dx = DirectX;

export namespace starfield
{
    template <
        std::floating_point float_t = float,
        std::integral       size_t = unsigned int
    >
    class StarFactory final
    {
        using unique_pstar = std::unique_ptr<entity::Star>;

    public:
        using Color_t = D2D1_COLOR_F;


    public:
        struct Settings final
        {
            Settings(const std::vector<Color_t>& colors)
                :
                m_colors(colors)
            {
                
            }

            Settings() = default;

            size_t m_starCount     = 500ull;
            size_t m_minFlareCount =   2ull;
            size_t m_maxFlareCount =  10ull;

            float_t m_worldWidth  = 12000.0f;
            float_t m_worldHeight = 10000.0f;

            float_t m_meanFlares = 6.5f;
            float_t m_devFlares  = 2.0f;

            float_t m_meanStarRadius = 160.0f;
            float_t m_devStarRadius  = 90.0f;
            float_t m_maxStarRadius  = 300.0f;
            float_t m_minStarRadius  = 40.0f;

            float_t m_meanStarInnerRatio = 0.4f;
            float_t m_devStarInnerRatio  = 0.25f;
            float_t m_maxStarInnerRatio  = 0.8f;
            float_t m_minStarInnerRatio  = 0.15f;

            float_t m_meanColorFrequency = 1.5f;
            float_t m_devColorFrequency  = 1.0f;
            float_t m_minColorFrequency  = 0.2f;
            float_t m_maxColorFrequency  = 1.0f;

            float_t m_meanRadiusAmplitude = 0.5f;
            float_t m_devRadiusAmplitude  = 0.3f;
            float_t m_minRadiusAmplitude  = 0.1f;
            float_t m_maxRadiusAmplitude  = 0.9f;

            float_t m_meanRadiusFrequency = 1.8f;
            float_t m_devRadiusFrequency  = 1.5f;
            float_t m_minRadiusFrequency  = 0.6f;
            float_t m_maxRadiusFrequency  = 4.0f;

            float_t m_minRotationSpeed = -1.0f * ::std::numbers::pi_v<float_t>;
            float_t m_maxRotationSpeed =  1.0f * ::std::numbers::pi_v<float_t>;

            std::vector<Color_t> m_colors =
            {
                ::D2D1::ColorF{ ::D2D1::ColorF::Red        },
                ::D2D1::ColorF{ ::D2D1::ColorF::Green      },
                ::D2D1::ColorF{ ::D2D1::ColorF::Blue       },
                ::D2D1::ColorF{ ::D2D1::ColorF::Cyan       },
                ::D2D1::ColorF{ ::D2D1::ColorF::Yellow     },
                ::D2D1::ColorF{ ::D2D1::ColorF::Magenta    },
                ::D2D1::ColorF{ ::D2D1::ColorF::SandyBrown },
                ::D2D1::ColorF{ ::D2D1::ColorF::Crimson    }
            };
        };


    public:
        explicit StarFactory(const Settings& settings = {})
            :
            mc_settings_(settings)
        {
            m_stars_.reserve(mc_settings_.m_starCount);

            while (m_stars_.size() < mc_settings_.m_starCount)
            {
                m_stars_.push_back(GenerateStar_());
            }
        }
        
        explicit StarFactory(const StarFactory&)     = delete;
        explicit StarFactory(StarFactory&&) noexcept = delete;

        auto operator = (const StarFactory&)     -> StarFactory& = delete;
        auto operator = (StarFactory&&) noexcept -> StarFactory& = delete;
        ~StarFactory() noexcept                                  = default;


    public:
        auto GetStars() && noexcept -> std::vector<unique_pstar>&&
        {
            return std::move(m_stars_);
        }


    protected:


    private:
        auto GenerateStar_() -> unique_pstar
        {
        generation:

            const auto starPosition = dx::XMFLOAT2{ m_dist_x_position_(m_rng_), m_dist_y_position_(m_rng_) };
            const auto radius       = std::clamp(m_dist_radius_(m_rng_), mc_settings_.m_minStarRadius, mc_settings_.m_maxStarRadius);
            const auto radAmplitude = std::clamp(m_dist_radius_amplitude_(m_rng_), mc_settings_.m_minRadiusAmplitude, mc_settings_.m_maxRadiusAmplitude);
            const auto maxRadius    = radius * (1.0f + radAmplitude);

            if (std::ranges::any_of(m_stars_, [&](const auto& pstar) noexcept -> bool { return pstar->IsWithinArea(starPosition, maxRadius); }))
            {
                goto generation;
            }

            entity::Star::Descriptor desc{
                .position              = starPosition,
                .radiuses              = { radius, radius * std::clamp(m_dist_radius_ratio_(m_rng_), mc_settings_.m_minStarInnerRatio, mc_settings_.m_maxStarInnerRatio) },
                .color                 = mc_settings_.m_colors[m_dist_color_(m_rng_)],
                .flareCount            = std::clamp(static_cast<size_t>(m_dist_flare_count_(m_rng_)), mc_settings_.m_minFlareCount, mc_settings_.m_maxFlareCount),
                .colorFrequency        = std::clamp(m_dist_color_frequency_(m_rng_), mc_settings_.m_minColorFrequency, mc_settings_.m_maxColorFrequency),
                .colorPhase            = m_dist_phase_(m_rng_),
                .radiusAmplitude       = radAmplitude,
                .radiusFrequency       = m_dist_radius_frequency_(m_rng_),
                .radiusPhase           = m_dist_phase_(m_rng_),
                .rotationSpeed         = m_dist_rotation_speed_(m_rng_)
            };

            return std::make_unique<entity::Star>(desc);
        }


    private:
        const Settings mc_settings_;

        std::minstd_rand m_rng_{ std::random_device{}() };
        std::uniform_int_distribution<std::size_t> m_dist_color_{ 0u, mc_settings_.m_colors.size() - 1u };
        std::uniform_real_distribution<float> m_dist_phase_{ 0.0f, 2.0f * std::numbers::pi_v<float> };

        std::uniform_real_distribution<float> m_dist_x_position_    { -mc_settings_.m_worldWidth / 2.0f, mc_settings_.m_worldWidth / 2.0f };
        std::uniform_real_distribution<float> m_dist_y_position_    { -mc_settings_.m_worldWidth / 2.0f, mc_settings_.m_worldWidth / 2.0f };
        std::uniform_real_distribution<float> m_dist_rotation_speed_{  mc_settings_.m_minRotationSpeed,  mc_settings_.m_maxRotationSpeed };

        std::normal_distribution<float> m_dist_radius_          { mc_settings_.m_meanStarRadius,      mc_settings_.m_devStarRadius };
        std::normal_distribution<float> m_dist_radius_ratio_    { mc_settings_.m_meanStarInnerRatio,  mc_settings_.m_devStarInnerRatio };
        std::normal_distribution<float> m_dist_radius_amplitude_{ mc_settings_.m_meanRadiusAmplitude, mc_settings_.m_devRadiusAmplitude};
        std::normal_distribution<float> m_dist_radius_frequency_{ mc_settings_.m_meanRadiusFrequency, mc_settings_.m_devRadiusFrequency};
        std::normal_distribution<float> m_dist_flare_count_     { mc_settings_.m_meanFlares,          mc_settings_.m_devFlares };
        std::normal_distribution<float> m_dist_color_frequency_ { mc_settings_.m_meanColorFrequency,  mc_settings_.m_devColorFrequency };

        std::vector<unique_pstar> m_stars_;
    };
}

module : private;