module;

#include <FatWin32_.hpp>

#include <DirectXMath.h>

#include <d2d1.h>

module StarField.View.CameraController;

namespace dx = DirectX;

namespace starfield
{
    CameraController::CameraController(Camera& camera, NAMESPACE_IO::Mouse& mouse, const NAMESPACE_IO::Keyboard& kbd)
        :
        camera_(camera),
        mouse_(mouse),
        kbd_(kbd)
    {

    }

    void CameraController::Update(float deltaTime)
    {
        if (kbd_.KeyIsPressed('Q'))
        {
            camera_.SetAngle(camera_.GetAngle() + rotationSpeed_ * deltaTime);
        }

        if (kbd_.KeyIsPressed('E'))
        {
            camera_.SetAngle(camera_.GetAngle() - rotationSpeed_ * deltaTime);
        }

        while (!mouse_.EventBufferIsEmpty())
        {
            const auto e = mouse_.ReadFromBuffer();

            switch (e.GetType())
            {
            case NAMESPACE_IO::Mouse::Event::Type::LPress:
            {
                engaged_ = true;
                const auto& pos = e.GetPos();
                lastPosition_ = { static_cast<float>(pos.first),  static_cast<float>(pos.second) };
            }
                break;

            case NAMESPACE_IO::Mouse::Event::Type::LRelease:
                engaged_ = false;
                break;

            case NAMESPACE_IO::Mouse::Event::Type::WheelUp:
                camera_.SetScale(camera_.GetScale() * zoomFactor_);
                break;

            case NAMESPACE_IO::Mouse::Event::Type::WheelDown:
                camera_.SetScale(camera_.GetScale() / zoomFactor_);
                break;

            default:
                break;
            }
        }

        if (engaged_)
        {
            dx::XMVECTOR lastPositionVec = dx::XMLoadFloat2(&lastPosition_);

            const auto& pos = mouse_.GetPos();
            dx::XMFLOAT2 currentPosition = { static_cast<float>(pos.first), static_cast<float>(pos.second) };

            dx::XMVECTOR currentPositionVec = dx::XMLoadFloat2(&currentPosition);
            dx::XMVECTOR deltaPositionVec = dx::XMVectorSubtract(currentPositionVec, lastPositionVec);

            deltaPositionVec = dx::XMVectorSetX(deltaPositionVec, -dx::XMVectorGetX(deltaPositionVec));

            float angle = -camera_.GetAngle();
            dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationZ(angle);
            deltaPositionVec = dx::XMVector2TransformCoord(deltaPositionVec, rotationMatrix);

            dx::XMVECTOR scaleVec = dx::XMVectorReplicate(1.0f / camera_.GetScale());
            deltaPositionVec = dx::XMVectorMultiply(deltaPositionVec, scaleVec);

            dx::XMFLOAT2 deltaPosition;
            dx::XMStoreFloat2(&deltaPosition, deltaPositionVec);

            camera_.MoveBy(deltaPosition);

            lastPosition_ = currentPosition;
        }
    }
}