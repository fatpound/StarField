module;

#include "../FatWin32_.hpp"

module FatPound.Win32.IO.Mouse;

namespace fatpound::win32::io
{
    // Mouse

    auto Mouse::GetPos() const noexcept -> std::pair<int, int>
    {
        return { x_, y_ };
    }

    auto Mouse::ReadRawDelta() noexcept -> std::optional<RawDelta>
    {
        if (raw_delta_buffer_.empty())
        {
            return std::nullopt;
        }

        const RawDelta delta = raw_delta_buffer_.front();
        raw_delta_buffer_.pop();

        return delta;
    }

    auto Mouse::ReadFromBuffer() noexcept -> Event
    {
        if (event_buffer_.size() > 0u)
        {
            Event e = event_buffer_.front();
            event_buffer_.pop();

            return e;
        }

        return {};
    }

    int Mouse::GetPosX() const noexcept
    {
        return x_;
    }
    int Mouse::GetPosY() const noexcept
    {
        return y_;
    }

    bool Mouse::EventBufferIsEmpty() const noexcept
    {
        return event_buffer_.empty();
    }
    bool Mouse::IsInWindow() const noexcept
    {
        return is_in_window_;
    }

    bool Mouse::LeftIsPressed() const noexcept
    {
        return left_is_pressed_;
    }
    bool Mouse::RightIsPressed() const noexcept
    {
        return right_is_pressed_;
    }
    bool Mouse::WheelIsPressed() const noexcept
    {
        return wheel_is_pressed_;
    }

    void Mouse::FlushBuffer() noexcept
    {
        event_buffer_ = std::queue<Event>{};
    }

    void Mouse::OnMouseMove_(int x, int y) noexcept
    {
        x_ = x;
        y_ = y;

        event_buffer_.push({ Event::Type::Move, *this });

        TrimBuffer_();
    }
    void Mouse::OnMouseEnter_() noexcept
    {
        is_in_window_ = true;

        event_buffer_.push({ Event::Type::Enter, *this });

        TrimBuffer_();
    }
    void Mouse::OnMouseLeave_() noexcept
    {
        is_in_window_ = false;

        event_buffer_.push({ Event::Type::Leave, *this });

        TrimBuffer_();
    }

    void Mouse::OnRawDelta_(int dx, int dy) noexcept
    {
        raw_delta_buffer_.push(RawDelta(dx, dy));

        TrimBuffer_();
    }

    void Mouse::OnLeftPressed_() noexcept
    {
        left_is_pressed_ = true;

        event_buffer_.push({ Event::Type::LPress, *this });

        TrimBuffer_();
    }
    void Mouse::OnLeftReleased_() noexcept
    {
        left_is_pressed_ = false;

        event_buffer_.push({ Event::Type::LRelease, *this });

        TrimBuffer_();
    }
    void Mouse::OnRightPressed_() noexcept
    {
        right_is_pressed_ = true;

        event_buffer_.push({ Event::Type::RPress, *this });

        TrimBuffer_();
    }
    void Mouse::OnRightReleased_() noexcept
    {
        right_is_pressed_ = false;

        event_buffer_.push({ Event::Type::RRelease, *this });

        TrimBuffer_();
    }
    void Mouse::OnWheelPressed_() noexcept
    {
        wheel_is_pressed_ = true;

        event_buffer_.push({ Event::Type::WheelPress, *this });

        TrimBuffer_();
    }
    void Mouse::OnWheelReleased_() noexcept
    {
        wheel_is_pressed_ = false;

        event_buffer_.push({ Event::Type::WheelRelease, *this });

        TrimBuffer_();
    }
    void Mouse::OnWheelUp_() noexcept
    {
        event_buffer_.push({ Event::Type::WheelUp, *this });

        TrimBuffer_();
    }
    void Mouse::OnWheelDown_() noexcept
    {
        event_buffer_.push({ Event::Type::WheelDown, *this });

        TrimBuffer_();
    }
    void Mouse::OnWheelDelta_(int delta) noexcept
    {
        wheel_delta_carry_ += delta;

        while (wheel_delta_carry_ >= WHEEL_DELTA)
        {
            wheel_delta_carry_ -= WHEEL_DELTA;

            OnWheelUp_();
        }

        while (wheel_delta_carry_ <= -WHEEL_DELTA)
        {
            wheel_delta_carry_ += WHEEL_DELTA;

            OnWheelDown_();
        }
    }

    void Mouse::TrimBuffer_() noexcept
    {
        while (event_buffer_.size() > Mouse::buffer_size_)
        {
            event_buffer_.pop();
        }
    }
    void Mouse::TrimRawInputBuffer_() noexcept
    {
        while (raw_delta_buffer_.size() > Mouse::buffer_size_)
        {
            raw_delta_buffer_.pop();
        }
    }


    // Event

    Mouse::Event::Event() noexcept
        :
        type_(Type::Invalid),
        x_(0),
        y_(0),
         left_is_pressed_(false),
        right_is_pressed_(false),
        wheel_is_pressed_(false)
    {

    }
    Mouse::Event::Event(Type type, const Mouse& parent) noexcept
        :
        type_(type),
        x_(parent.x_),
        y_(parent.y_),
         left_is_pressed_(parent.left_is_pressed_),
        right_is_pressed_(parent.right_is_pressed_),
        wheel_is_pressed_(parent.wheel_is_pressed_)
    {

    }

    auto Mouse::Event::GetPos() const noexcept -> std::pair<int, int>
    {
        return { x_, y_ };
    }

    auto Mouse::Event::GetType() const noexcept -> Type
    {
        return type_;
    }

    int Mouse::Event::GetPosX() const noexcept
    {
        return x_;
    }
    int Mouse::Event::GetPosY() const noexcept
    {
        return y_;
    }

    bool Mouse::Event::IsValid() const noexcept
    {
        return type_ != Type::Invalid;
    }
    bool Mouse::Event::LeftIsPressed() const noexcept
    {
        return left_is_pressed_;
    }
    bool Mouse::Event::RightIsPressed() const noexcept
    {
        return right_is_pressed_;
    }
    bool Mouse::Event::WheelIsPressed() const noexcept
    {
        return wheel_is_pressed_;
    }
}