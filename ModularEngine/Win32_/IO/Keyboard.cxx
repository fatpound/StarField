module;

module FatPound.Win32.IO.Keyboard;

namespace fatpound::win32::io
{
    // Keyboard

    auto Keyboard::ReadKeyFromBuffer() noexcept -> Event
    {
        if (event_buffer_.size() > 0u)
        {
            Event e = event_buffer_.front();
            event_buffer_.pop();

            return e;
        }

        return {};
    }

    char Keyboard::ReadCharFromBuffer() noexcept
    {
        if (char_buffer_.size() > 0u)
        {
            unsigned char charcode = char_buffer_.front();
            char_buffer_.pop();

            return charcode;
        }

        return 0;
    }

    bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
    {
        return key_states_[keycode];
    }
    bool Keyboard::KeyBufferIsEmpty() const noexcept
    {
        return event_buffer_.empty();
    }
    bool Keyboard::CharBufferIsEmpty() const noexcept
    {
        return char_buffer_.empty();
    }
    bool Keyboard::AutoRepeatIsEnabled() const noexcept
    {
        return auto_repeat_enabled_;
    }

    void Keyboard::FlushKeyBuffer() noexcept
    {
        event_buffer_ = std::queue<Event>{};
    }
    void Keyboard::FlushCharBuffer() noexcept
    {
        char_buffer_ = std::queue<char>{};
    }
    void Keyboard::FlushBuffers() noexcept
    {
        FlushKeyBuffer();
        FlushCharBuffer();
    }
    void Keyboard::EnableAutoRepeat() noexcept
    {
        auto_repeat_enabled_ = true;
    }
    void Keyboard::DisableAutoRepeat() noexcept
    {
        auto_repeat_enabled_ = false;
    }

    void Keyboard::OnKeyPressed_(unsigned char keycode) noexcept
    {
        key_states_[keycode] = true;

        event_buffer_.push({ Event::Type::Press, keycode });

        TrimBuffer_(event_buffer_);
    }
    void Keyboard::OnKeyReleased_(unsigned char keycode) noexcept
    {
        key_states_[keycode] = false;

        event_buffer_.push({ Event::Type::Release, keycode });

        TrimBuffer_(event_buffer_);
    }
    void Keyboard::OnChar_(char character) noexcept
    {
        char_buffer_.push(character);

        TrimBuffer_(char_buffer_);
    }

    void Keyboard::ClearKeyStateBitset_() noexcept
    {
        key_states_.reset();
    }


    // Event

    Keyboard::Event::Event() noexcept
        :
        type_(Type::Invalid),
        code_(0u)
    {

    }
    Keyboard::Event::Event(Type type, unsigned char code) noexcept
        :
        type_(type),
        code_(code)
    {

    }

    unsigned char Keyboard::Event::GetCode() const noexcept
    {
        return code_;
    }

    bool Keyboard::Event::IsPress() const noexcept
    {
        return type_ == Type::Press;
    }
    bool Keyboard::Event::IsRelease() const noexcept
    {
        return type_ == Type::Release;
    }
    bool Keyboard::Event::IsInvalid() const noexcept
    {
        return type_ == Type::Invalid;
    }
}