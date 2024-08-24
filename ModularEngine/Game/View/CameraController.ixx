module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

export module StarField.View.CameraController;

import StarField.View.Camera;

import FatPound;

import std;

export namespace starfield
{
    class CameraController final
    {
    public:
        CameraController(Camera& camera, NAMESPACE_IO::Mouse& mouse, const NAMESPACE_IO::Keyboard& kbd);


    public:
        void Update(float deltaTime);


    protected:


    private:
        Camera& camera_;

        NAMESPACE_IO::Mouse& mouse_;
        const NAMESPACE_IO::Keyboard& kbd_;

        DirectX::XMFLOAT2 lastPosition_ = { 0.0f, 0.0f };

        bool engaged_ = false;

        static constexpr float zoomFactor_ = 1.05f;
        static constexpr float rotationSpeed_ = DirectX::XM_PI / 6.0f;
    };
}