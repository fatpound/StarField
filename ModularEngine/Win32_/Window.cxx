module;

#include "FatWin32_.hpp"

#define WINDOW_RECT_WIDTH       rect.right  - rect.left
#define WINDOW_RECT_HEIGHT      rect.bottom - rect.top
                                
#define CLIENT_WIDTH            client_size_.width
#define CLIENT_HEIGHT           client_size_.height
                                
#if IN_RELEASE                  
                                
#define WINDOW_WIDTH            CLIENT_WIDTH
#define WINDOW_HEIGHT           CLIENT_HEIGHT
#define WINDOW_STYLE            WS_POPUP
#define WINDOW_POSITION_X       CW_USEDEFAULT
#define WINDOW_POSITION_Y       CW_USEDEFAULT
                                
#else                           
                                
#define WINDOW_WIDTH            WINDOW_RECT_WIDTH
#define WINDOW_HEIGHT           WINDOW_RECT_HEIGHT
#define WINDOW_STYLE            WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_POSITION_X       rect.left
#define WINDOW_POSITION_Y       rect.top

#endif // IN_RELEASE

module FatPound.Win32.Window;

namespace fatpound::win32
{
    Window::Window(const wchar_t* const title, const ClientSizeInfo& dimensions)
        :
        client_size_{ dimensions }
    {
#if IN_DEBUG

        RECT rect = {};
        rect.left = 150;
        rect.right = CLIENT_WIDTH + rect.left;
        rect.top = 150;
        rect.bottom = CLIENT_HEIGHT + rect.top;

        AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

#endif // IN_DEBUG

        hWnd_ = CreateWindow(
            WndClass_::GetName(),
            title,
            WINDOW_STYLE,
            WINDOW_POSITION_X,
            WINDOW_POSITION_Y,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            nullptr,
            nullptr,
            WndClass_::GetInstance(),
            this
        );

        if (hWnd_ == nullptr) [[unlikely]]
        {
            throw std::runtime_error("Window cannot be created!");
        }

        ShowWindow(hWnd_, SW_SHOWDEFAULT);
        UpdateWindow(hWnd_);
    }
    Window::~Window()
    {
        DestroyWindow(hWnd_);
    }

    auto Window::ProcessMessages() noexcept -> std::optional<WPARAM>
    {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) [[unlikely]]
            {
                return msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return std::nullopt;
    }

    auto Window::GetHwnd() const noexcept -> HWND
    {
        return hWnd_;
    }

    void Window::SetTitle(const std::wstring& title)
    {
        if (SetWindowText(hWnd_, title.c_str()) == 0)
        {
            throw std::runtime_error("Window text cannot be changed!");
        }
    }

    void Window::Kill()
    {
        PostQuitMessage(0);
    }

    LRESULT CALLBACK Window::HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk_));

            return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    LRESULT CALLBACK Window::HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
    }

    LRESULT Window::HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_KILLFOCUS:
            kbd.ClearKeyStateBitset_();
            break;

            /******** KEYBOARD MESSAGES ********/
        case WM_KEYDOWN: [[fallthrough]];
        case WM_SYSKEYDOWN:
            if ( ! (lParam & 0x40000000) || kbd.AutoRepeatIsEnabled())
            {
                kbd.OnKeyPressed_(static_cast<unsigned char>(wParam));
            }
            break;

        case WM_KEYUP: [[fallthrough]];
        case WM_SYSKEYUP:
            kbd.OnKeyReleased_(static_cast<unsigned char>(wParam));
            break;

        case WM_CHAR:
            kbd.OnChar_(static_cast<unsigned char>(wParam));
            break;
            /******** END KEYBOARD MESSAGES ********/


            /******** MOUSE  MESSAGES ********/
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);

            if (pt.x >= 0 && pt.x < client_size_.width && pt.y >= 0 && pt.y < client_size_.height)
            {
                mouse.OnMouseMove_(pt.x, pt.y);

                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);

                    mouse.OnMouseEnter_();
                }
            }
            else
            {
                if (wParam & (MK_LBUTTON | MK_RBUTTON))
                {
                    mouse.OnMouseMove_(pt.x, pt.y);
                }
                else
                {
                    ReleaseCapture();

                    mouse.OnMouseLeave_();
                }
            }
        }
            break;

        case WM_LBUTTONDOWN:
            mouse.OnLeftPressed_();
            break;

        case WM_LBUTTONUP:
            mouse.OnLeftReleased_();
            break;

        case WM_RBUTTONDOWN:
            mouse.OnRightPressed_();
            break;

        case WM_RBUTTONUP:
            mouse.OnRightReleased_();
            break;

        case WM_MBUTTONDOWN:
            mouse.OnWheelPressed_();
            break;

        case WM_MBUTTONUP:
            mouse.OnWheelReleased_();
            break;

        case WM_MOUSEWHEEL:
            mouse.OnWheelDelta_(GET_WHEEL_DELTA_WPARAM(wParam));
            break;
            /******** END MOUSE MESSAGES ********/

        default:
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }


    // WndClass_

    Window::WndClass_::WndClass_() noexcept
        :
        hInst_(GetModuleHandle(nullptr))
    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = &Window::HandleMsgSetup_;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInst_;
        wc.hIcon = nullptr;
        wc.hIconSm = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = WndClass_::wndClassName_;

        if constexpr (Window::cursor_enabled_)
        {
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        }
        else
        {
            ShowCursor(false);
        }

        RegisterClassEx(&wc);
    }
    Window::WndClass_::~WndClass_()
    {
        UnregisterClass(WndClass_::wndClassName_, WndClass_::GetInstance());
    }

    HINSTANCE Window::WndClass_::GetInstance() noexcept
    {
        static WndClass_ wndClass_;

        return wndClass_.hInst_;
    }

    const wchar_t* const Window::WndClass_::GetName() noexcept
    {
        return wndClassName_;
    }
}