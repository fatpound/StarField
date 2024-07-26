module;

#include "../FatWin32_Namespaces.hpp"

export module FatPound.Win32.IO.Keyboard;

import std;

namespace fatpound::win32
{
    class Window;
}

export namespace fatpound::win32::io
{
    class Keyboard final
    {
        friend class NAMESPACE_WIN32::Window;

    public:
        Keyboard() = default;
        Keyboard(const Keyboard& src) = delete;
        Keyboard& operator = (const Keyboard& src) = delete;
        Keyboard(Keyboard&& src) = delete;
        Keyboard& operator = (Keyboard&& src) = delete;
        ~Keyboard() noexcept = default;


    public:
        class Event final
        {
        public:
            enum class Type
            {
                Press,
                Release,
                Invalid
            };

        public:
            Event() noexcept;

            Event(Type type, unsigned char code) noexcept;

        public:
            unsigned char GetCode() const noexcept;

            bool IsPress() const noexcept;
            bool IsRelease() const noexcept;
            bool IsInvalid() const noexcept;

        protected:

        private:
            Type type_;

            unsigned char code_;
        };


    public:
        Event ReadKeyFromBuffer() noexcept;

        char ReadCharFromBuffer() noexcept;

        bool KeyIsPressed(unsigned char keycode) const noexcept;
        bool KeyBufferIsEmpty() const noexcept;
        bool CharBufferIsEmpty() const noexcept;
        bool AutoRepeatIsEnabled() const noexcept;

        void FlushKeyBuffer() noexcept;
        void FlushCharBuffer() noexcept;
        void FlushBuffers() noexcept;
        void EnableAutoRepeat() noexcept;
        void DisableAutoRepeat() noexcept;


    protected:


    private:
        template <typename T>
        static void TrimBuffer_(std::queue<T>& buffer) noexcept
        {
            while (buffer.size() > Keyboard::buffer_size_)
            {
                buffer.pop();
            }
        }


    private:
        void OnKeyPressed_(unsigned char keycode) noexcept;
        void OnKeyReleased_(unsigned char keycode) noexcept;
        void OnChar_(char character) noexcept;

        void ClearKeyStateBitset_() noexcept;


    private:
        static constexpr unsigned int key_count_ = 256u;

        std::bitset<Keyboard::key_count_> key_states_;

        std::queue<Event> event_buffer_;
        std::queue<char>   char_buffer_;

        bool auto_repeat_enabled_ = false;

        static constexpr unsigned int buffer_size_ = 16u;
    };
}