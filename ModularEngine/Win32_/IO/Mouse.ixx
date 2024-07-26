module;

#include "../FatWin32_Namespaces.hpp"

export module FatPound.Win32.IO.Mouse;

import std;

namespace fatpound::win32
{
    class Window;
}

export namespace fatpound::win32::io
{
    class Mouse final
    {
        friend class NAMESPACE_WIN32::Window;

    public:
        Mouse() = default;
        Mouse(const Mouse& src) = delete;
        Mouse& operator = (const Mouse& src) = delete;
        Mouse(Mouse&& src) = delete;
        Mouse& operator = (Mouse&& src) = delete;
        ~Mouse() noexcept = default;


    public:
        struct RawDelta final
        {
            int x;
            int y;
        };


    public:
        class Event final
        {
        public:
            enum class Type
            {
                LPress,
                LRelease,
                RPress,
                RRelease,
                WheelPress,
                WheelRelease,
                WheelUp,
                WheelDown,
                Enter,
                Move,
                Leave,
                Invalid
            };

        public:
            Event() noexcept;

            Event(Type type, const Mouse& parent) noexcept;

        public:
            auto GetPos() const noexcept -> std::pair<int, int>;

            Type GetType() const noexcept;

            int GetPosX() const noexcept;
            int GetPosY() const noexcept;

            bool IsValid() const noexcept;
            bool LeftIsPressed() const noexcept;
            bool RightIsPressed() const noexcept;
            bool WheelIsPressed() const noexcept;

        protected:

        private:
            Type type_;

            int x_;
            int y_;

            bool  left_is_pressed_;
            bool right_is_pressed_;
            bool wheel_is_pressed_;
        };


    public:
        auto GetPos() const noexcept -> std::pair<int, int>;

        auto ReadRawDelta() noexcept -> std::optional<RawDelta>;

        Event ReadFromBuffer() noexcept;

        int GetPosX() const noexcept;
        int GetPosY() const noexcept;

        bool EventBufferIsEmpty() const noexcept;
        bool IsInWindow() const noexcept;

        bool  LeftIsPressed() const noexcept;
        bool RightIsPressed() const noexcept;
        bool WheelIsPressed() const noexcept;

        void FlushBuffer() noexcept;


    protected:


    private:
        void OnMouseMove_(int x, int y) noexcept;
        void OnMouseEnter_() noexcept;
        void OnMouseLeave_() noexcept;

        void OnRawDelta_(int dx, int dy) noexcept;

        void OnLeftPressed_() noexcept;
        void OnLeftReleased_() noexcept;
        void OnRightPressed_() noexcept;
        void OnRightReleased_() noexcept;
        void OnWheelPressed_() noexcept;
        void OnWheelReleased_() noexcept;
        void OnWheelUp_() noexcept;
        void OnWheelDown_() noexcept;
        void OnWheelDelta_(int delta) noexcept;

        void TrimBuffer_() noexcept;
        void TrimRawInputBuffer_() noexcept;


    private:
        std::queue<Event> event_buffer_;
        std::queue<RawDelta> raw_delta_buffer_;

        int x_ = 0;
        int y_ = 0;
        int wheel_delta_carry_ = 0;

        bool is_in_window_ = false;

        bool  left_is_pressed_ = false;
        bool right_is_pressed_ = false;
        bool wheel_is_pressed_ = false;

        static constexpr unsigned int buffer_size_ = 16u;
    };
}