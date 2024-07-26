module;

#include <DirectXMath.h>

export module FatPound.Math;

export import :RectF;

import std;

export namespace fatpound::math
{
    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    template <typename T>
    inline auto Square(const T& x)
    {
        return x * x;
    }

    float GetDistanceBetweenXMF2(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
    {
        const auto& pos1Vec = DirectX::XMLoadFloat2(&pos1);
        const auto& pos2Vec = DirectX::XMLoadFloat2(&pos2);

        const auto& differenceVec = DirectX::XMVectorSubtract(pos1Vec, pos2Vec);

        return DirectX::XMVectorGetX(
            DirectX::XMVector2Length(differenceVec)
        );
    }
}