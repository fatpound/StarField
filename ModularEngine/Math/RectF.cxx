module;

#include <DirectXMath.h>

#include <d2d1.h>

module FatPound.Math:RectF;

namespace fatpound::math
{
    RectF::RectF(float left, float top, float right, float bottom)
        :
        rect{ left, top, right, bottom }
    {

    }
    RectF::RectF(const DirectX::XMFLOAT2& topLeft, float width, float height)
        :
        RectF(D2D1_POINT_2F{ topLeft.x, topLeft.y }, width, height)
    {

    }
    RectF::RectF(const D2D1_POINT_2F& topLeft, float width, float height)
        :
        rect{ topLeft.x, topLeft.y, topLeft.x + width, topLeft.y + height }
    {

    }
    RectF::RectF(const D2D1_POINT_2F& topLeft, const D2D1_POINT_2F& bottomRight)
        :
        rect{ topLeft.x, topLeft.y, bottomRight.x, bottomRight.y }
    {

    }

    RectF RectF::FromCenter(const D2D1_POINT_2F& center, const float halfWidth, const float halfHeight)
    {
        return RectF(center.x - halfWidth, center.y - halfHeight, center.x + halfWidth, center.y + halfHeight);
    }
    RectF RectF::FromCenter(const DirectX::XMFLOAT2& center, const float halfWidth, const float halfHeight)
    {
        return RectF(center.x - halfWidth, center.y - halfHeight, center.x + halfWidth, center.y + halfHeight);
    }
    RectF RectF::GetExpanded(float offset) const
    {
        return RectF(rect.left - offset, rect.top - offset, rect.right + offset, rect.bottom + offset);
    }

    D2D1_POINT_2F RectF::GetCenter() const
    {
        return D2D1_POINT_2F{ (rect.left + rect.right) / 2.0f, (rect.top + rect.bottom) / 2.0f };
    }

    bool RectF::IsOverlappingWith(const RectF& other) const
    {
        return rect.right > other.rect.left && rect.left < other.rect.right &&
            rect.bottom > other.rect.top && rect.top < other.rect.bottom;
    }
    bool RectF::IsContainedBy(const RectF& other) const
    {
        return rect.left >= other.rect.left && rect.right <= other.rect.right &&
            rect.top >= other.rect.top && rect.bottom <= other.rect.bottom;
    }
}