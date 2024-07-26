/*          */
// fatpound //
/*          */

#include "Win32_/FatWin32_.hpp"

import StarField;

int APIENTRY wWinMain(
    [[maybe_unused]] _In_     HINSTANCE hInst,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInst,
    [[maybe_unused]] _In_     LPWSTR    lpCmdLine,
    [[maybe_unused]] _In_     int       nShowCmd)
{
    return starfield::Game{}.Go();
}