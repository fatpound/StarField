module;

export module StarField.View.Rectangle;

import FatPound;

import std;

export namespace starfield::view
{
    template <fatpound::traits::IntegralOrFloating T>
    class Rectangle
    {
    public:
        explicit Rectangle(const T& left, const T& top, const T& right, const T& bottom) noexcept
            :
            m_left_(left),
            m_top_(top),
            m_right_(right),
            m_bottom_(bottom)
        {

        }

        explicit Rectangle()            = delete;
        Rectangle(const Rectangle&)     = default;
        Rectangle(Rectangle&&) noexcept = default;

        auto operator = (const Rectangle&)     -> Rectangle& = default;
        auto operator = (Rectangle&&) noexcept -> Rectangle& = default;
        ~Rectangle() noexcept                                = default;


    public:
        static auto FromCenter(const T& centerX, const T& centerY, const T& halfWidth, const T& halfHeight) noexcept -> Rectangle
        {
            return Rectangle(centerX - halfWidth, centerY - halfHeight, centerX + halfWidth, centerY + halfHeight);
        }


    public:
        auto GetCenter() const noexcept -> std::pair<T, T>
        {
            return
            {
                (m_left_ + m_right_)  / static_cast<T>(2.0),
                (m_top_  + m_bottom_) / static_cast<T>(2.0)
            };
        }

        auto GetExpanded(const T& offset) const noexcept -> Rectangle
        {
            return Rectangle(m_left_ - offset, m_top_ - offset, m_right_ + offset, m_bottom_ + offset);
        }

        auto IsOverlappingWith(const Rectangle& other) const noexcept -> bool
        {
            return     m_left_   < other.m_right_
                   and m_top_    < other.m_bottom_
                   and m_right_  > other.m_left_
                   and m_bottom_ > other.m_top_;
        }
        auto IsContainedBy(const Rectangle& other) const noexcept -> bool
        {
            return     m_left_   >= other.m_left_
                   and m_top_    >= other.m_top_
                   and m_right_  <= other.m_right_
                   and m_bottom_ <= other.m_bottom_;
        }


    protected:


    private:
        T m_left_;
        T m_top_;
        T m_right_;
        T m_bottom_;
    };

    template <fatpound::traits::IntegralOrFloating T>
    using Rect = Rectangle<T>;
}

module : private;
