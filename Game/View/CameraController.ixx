module;

#include <FatNamespaces.hxx>

#include <d2d1.h>

export module StarField.View.CameraController;

import <DirectXMath.h>;

import StarField.View.Camera;

import FatPound;

import std;

namespace dx = DirectX;

export namespace starfield::view
{
    class CameraController final
    {
    public:
        explicit CameraController(Camera& camera, FATSPACE_IO::Mouse& mouse, const FATSPACE_IO::Keyboard& kbd) noexcept
            :
            m_camera_(camera),
            m_mouse_(mouse),
            m_kbd_(kbd)
        {

        }

        explicit CameraController()                            = delete;
        explicit CameraController(const CameraController&)     = delete;
        explicit CameraController(CameraController&&) noexcept = delete;

        auto operator = (const CameraController&)     -> CameraController& = delete;
        auto operator = (CameraController&&) noexcept -> CameraController& = delete;
        ~CameraController() noexcept                                       = default;


    public:
        void Update(const float deltaTime) noexcept
        {
            if (m_kbd_.KeyIsPressed('Q'))
            {
                m_camera_.SetAngle(m_camera_.GetAngle() + rotationSpeed_ * deltaTime);
            }

            if (m_kbd_.KeyIsPressed('E'))
            {
                m_camera_.SetAngle(m_camera_.GetAngle() - rotationSpeed_ * deltaTime);
            }

            while (not m_mouse_.EventBufferIsEmpty())
            {
                auto mouseE = m_mouse_.GetEvent();

                switch (mouseE->type)
                {
                case FATSPACE_IO::MouseEvent::Type::LPress:
                {
                    engaged_ = true;
                    m_last_pos_ = dx::XMFLOAT2
                    {
                        static_cast<float>(mouseE->pos_x),
                        static_cast<float>(mouseE->pos_y)
                    };
                }
                break;

                case FATSPACE_IO::MouseEvent::Type::LRelease:
                    engaged_ = false;
                    break;

                case FATSPACE_IO::MouseEvent::Type::WheelUp:
                    m_camera_.SetScale(m_camera_.GetScale() * zoomFactor_);
                    break;

                case FATSPACE_IO::MouseEvent::Type::WheelDown:
                    m_camera_.SetScale(m_camera_.GetScale() / zoomFactor_);
                    break;

                default:
                    break;
                }
            }

            if (engaged_)
            {
                const auto& pos = m_mouse_.GetPos();

                const auto& lastPositionVec = dx::XMLoadFloat2(&m_last_pos_);
                const auto& currentPosition = dx::XMFLOAT2{ static_cast<float>(pos.first), static_cast<float>(pos.second) };

                const auto& currentPositionVec = dx::XMLoadFloat2(&currentPosition);
                auto deltaPositionVec = dx::XMVectorSubtract(currentPositionVec, lastPositionVec);

                deltaPositionVec = dx::XMVectorSetX(deltaPositionVec, -dx::XMVectorGetX(deltaPositionVec));

                const auto angle = -m_camera_.GetAngle();

                const auto& rotationMatrix = dx::XMMatrixRotationZ(angle);

                deltaPositionVec = dx::XMVector2TransformCoord(deltaPositionVec, rotationMatrix);

                const auto& scaleVec = dx::XMVectorReplicate(1.0f / m_camera_.GetScale());
                deltaPositionVec = dx::XMVectorMultiply(deltaPositionVec, scaleVec);

                dx::XMFLOAT2 deltaPosition;
                dx::XMStoreFloat2(&deltaPosition, deltaPositionVec);

                m_camera_.MoveBy(deltaPosition);

                m_last_pos_ = currentPosition;
            }
        }


    protected:


    private:
        Camera& m_camera_;

        FATSPACE_IO::Mouse& m_mouse_;
        const FATSPACE_IO::Keyboard& m_kbd_;

        dx::XMFLOAT2 m_last_pos_{ 0.0f, 0.0f };

        bool engaged_{};

        static constexpr auto zoomFactor_ = 1.05f;
        static constexpr auto rotationSpeed_ = dx::XM_PI / 6.0f;
    };
}
