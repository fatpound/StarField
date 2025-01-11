/*          */
// fatpound //
/*          */

#include <FatWin32.hpp>

import StarField;

import std;

using std::literals::chrono_literals::operator ""ms;

int APIENTRY wWinMain(
    [[maybe_unused]] _In_     HINSTANCE hInst,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInst,
    [[maybe_unused]] _In_     LPWSTR    lpCmdLine,
    [[maybe_unused]] _In_     int       nShowCmd)
{
#if IN_DEBUG
    constexpr auto gameCount = 4;
#else
    constexpr auto gameCount = 1;
#endif // IN_DEBUG

    std::vector<std::unique_ptr<starfield::Game>> games;

    games.reserve(gameCount);

    for (auto i = 0; i < gameCount; ++i)
    {
        games.push_back(std::make_unique<starfield::Game>());
    }

    while (not games.empty())
    {
        std::erase_if(games, [](auto& pGame) noexcept -> bool { return pGame->IsOver(); });

        std::this_thread::sleep_for(100ms);
    }

    return 0;
}