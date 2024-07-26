module;

export module FatPound.Util.AutoTimer;

import std;

export namespace fatpound::util
{
    class AutoTimer final
    {
    public:
        float Mark() noexcept;
        float Peek() const noexcept;


    protected:


    private:
        std::chrono::steady_clock::time_point last_ = std::chrono::steady_clock::now();
    };
}