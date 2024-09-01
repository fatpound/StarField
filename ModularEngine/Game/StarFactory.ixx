module;

#include <FatWin32_Settings.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.StarFactory;

import StarField.Entity.Star;

import std;

namespace dx = DirectX;

export namespace starfield
{
    class StarFactory final
    {
    public:
        StarFactory();
        StarFactory(const StarFactory& src) = delete;
        StarFactory(StarFactory&& src) = delete;

        StarFactory& operator = (const StarFactory& src) = delete;
        StarFactory& operator = (StarFactory&& src) = delete;
        ~StarFactory() noexcept = default;


    public:
        auto GetStars() && noexcept -> std::vector<std::unique_ptr<entity::Star>> &&;


    protected:


    private:
        struct Settings_ final
        {
            static constexpr auto starCount_ = 500ull;
            static constexpr auto minFlareCount_ = 2ull;
            static constexpr auto maxFlareCount_ = 10ull;

            static constexpr auto worldWidth_ = 12000.0f;
            static constexpr auto worldHeight_ = 10000.0f;

            static constexpr auto meanFlares_ = 6.5f;
            static constexpr auto devFlares_ = 2.0f;

            static constexpr auto meanStarRadius_ = 160.0f;
            static constexpr auto devStarRadius_ = 90.0f;
            static constexpr auto maxStarRadius_ = 300.0f;
            static constexpr auto minStarRadius_ = 40.0f;

            static constexpr auto meanStarInnerRatio_ = 0.4f;
            static constexpr auto devStarInnerRatio_ = 0.25f;
            static constexpr auto maxStarInnerRatio_ = 0.8f;
            static constexpr auto minStarInnerRatio_ = 0.15f;

            static constexpr auto meanColorFrequency_ = 1.5f;
            static constexpr auto devColorFrequency_ = 1.0f;
            static constexpr auto minColorFrequency_ = 0.2f;
            static constexpr auto maxColorFrequency_ = 1.0f;

            static constexpr auto meanRadiusAmplitude_ = 0.5f;
            static constexpr auto devRadiusAmplitude_ = 0.3f;
            static constexpr auto minRadiusAmplitude_ = 0.1f;
            static constexpr auto maxRadiusAmplitude_ = 0.9f;

            static constexpr auto meanRadiusFrequency_ = 1.8f;
            static constexpr auto devRadiusFrequency_ = 1.5f;
            static constexpr auto minRadiusFrequency_ = 0.6f;
            static constexpr auto maxRadiusFrequency_ = 4.0f;

            static constexpr auto minRotationSpeed_ = -1.0f * std::numbers::pi_v<float>;
            static constexpr auto maxRotationSpeed_ =  1.0f * std::numbers::pi_v<float>;
        };


    private:
        auto GenerateStar_() -> std::unique_ptr<entity::Star>;


    private:
        std::vector<std::unique_ptr<entity::Star>> stars_;

        const std::vector<D2D1_COLOR_F> colors =
        {
            D2D1::ColorF{ D2D1::ColorF::Red        },
            D2D1::ColorF{ D2D1::ColorF::Green      },
            D2D1::ColorF{ D2D1::ColorF::Blue       },
            D2D1::ColorF{ D2D1::ColorF::Cyan       },
            D2D1::ColorF{ D2D1::ColorF::Yellow     },
            D2D1::ColorF{ D2D1::ColorF::Magenta    },
            D2D1::ColorF{ D2D1::ColorF::SandyBrown },
            D2D1::ColorF{ D2D1::ColorF::Crimson    }
        };

        std::minstd_rand rng_{ std::random_device{}() };
        std::uniform_int_distribution<std::size_t> dist_color_{ 0u, colors.size() - 1u };
        std::uniform_real_distribution<float> dist_phase_{ 0.0f, 2.0f * std::numbers::pi_v<float> };

        std::uniform_real_distribution<float> dist_x_position_    { -Settings_::worldWidth_ / 2.0f, Settings_::worldWidth_ / 2.0f };
        std::uniform_real_distribution<float> dist_y_position_    { -Settings_::worldWidth_ / 2.0f, Settings_::worldWidth_ / 2.0f };
        std::uniform_real_distribution<float> dist_rotation_speed_{  Settings_::minRotationSpeed_,  Settings_::maxRotationSpeed_ };

        std::normal_distribution<float> dist_radius_          { Settings_::meanStarRadius_,      Settings_::devStarRadius_ };
        std::normal_distribution<float> dist_radius_ratio_    { Settings_::meanStarInnerRatio_,  Settings_::devStarInnerRatio_ };
        std::normal_distribution<float> dist_radius_amplitude_{ Settings_::meanRadiusAmplitude_, Settings_::devRadiusAmplitude_};
        std::normal_distribution<float> dist_radius_frequency_{ Settings_::meanRadiusFrequency_, Settings_::devRadiusFrequency_};
        std::normal_distribution<float> dist_flare_count_     { Settings_::meanFlares_,          Settings_::devFlares_ };
        std::normal_distribution<float> dist_color_frequency_ { Settings_::meanColorFrequency_,  Settings_::devColorFrequency_ };
    };
}