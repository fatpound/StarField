module;

#include "../Win32_/FatWin32_.hpp"

#include <DirectXMath.h>

#include <d2d1.h>

export module FatPound.Math:RectF;

import std;

export namespace fatpound::math
{
    class RectF final
    {
    public:
        RectF() = default;

        RectF(float left, float top, float right, float bottom);
        RectF(const DirectX::XMFLOAT2& topLeft, float width, float height);
        RectF(const D2D1_POINT_2F& topLeft, float width, float height);
        RectF(const D2D1_POINT_2F& topLeft, const D2D1_POINT_2F& bottomRight);


    public:
        static RectF FromCenter(const D2D1_POINT_2F& center, const float halfWidth, const float halfHeight);
        static RectF FromCenter(const DirectX::XMFLOAT2& center, const float halfWidth, const float halfHeight);


    public:
        RectF GetExpanded(float offset) const;

        D2D1_POINT_2F GetCenter() const;

        bool IsOverlappingWith(const RectF& other) const;
        bool IsContainedBy(const RectF& other) const;


    public:
        D2D1_RECT_F rect;


    protected:


    private:
    };
}