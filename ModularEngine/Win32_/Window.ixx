module;

#include "FatWin32_.hpp"

export module FatPound.Win32.Window;

import FatPound.Win32.IO;
import FatPound.Math;

import std;

export namespace fatpound::win32
{
    class Window final
    {
    public:
        struct ClientSizeInfo final
        {
            int width;
            int height;
        };


    public:
        Window(const wchar_t* const title, const ClientSizeInfo& dimensions);

        Window() = delete;
        Window(const Window& src) = delete;
        Window& operator = (const Window& src) = delete;

        Window(Window&& src) = delete;
        Window& operator = (Window&& src) = delete;
        ~Window() noexcept;


    public:
        static auto ProcessMessages() noexcept -> std::optional<WPARAM>;


    public:
        template <NAMESPACE_MATH::Number N>
        auto GetClientWidth() const noexcept -> N
        {
            return static_cast<N>(client_size_.width);
        }

        template <NAMESPACE_MATH::Number N>
        auto GetClientHeight() const noexcept -> N
        {
            return static_cast<N>(client_size_.height);
        }

        auto GetHwnd() const noexcept -> HWND;

        void SetTitle(const std::wstring& title);
        void Kill();


    public:
        NAMESPACE_IO::Keyboard kbd;
        NAMESPACE_IO::Mouse mouse;


    protected:


    private:
        class WndClass_ final
        {
        public:
            static HINSTANCE GetInstance() noexcept;

            static const wchar_t* const GetName() noexcept;

        protected:

        private:
            WndClass_() noexcept;
            WndClass_(const WndClass_& src) = delete;
            WndClass_& operator = (const WndClass_& src) = delete;
            WndClass_(WndClass_&& src) = delete;
            WndClass_& operator = (WndClass_&& src) = delete;
            ~WndClass_();

        private:
            HINSTANCE hInst_;

            static constexpr auto wndClassName_ = L"FatPound D2D Engine Window Class";
        };


    private:
        static LRESULT CALLBACK HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    private:
        LRESULT HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    private:
        HWND hWnd_ = nullptr;

        const ClientSizeInfo client_size_;

        static constexpr bool cursor_enabled_ = true;
    };
}