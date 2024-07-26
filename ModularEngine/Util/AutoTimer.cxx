module;

module FatPound.Util.AutoTimer;

namespace fatpound::util
{
    float AutoTimer::Mark() noexcept
    {
        const auto old = last_;
        last_ = std::chrono::steady_clock::now();

        const std::chrono::duration<float> frameTime = last_ - old;

        return frameTime.count();
    }
    float AutoTimer::Peek() const noexcept
    {
        return std::chrono::duration<float>(std::chrono::steady_clock::now() - last_).count();
    }
}