#pragma once
#include <ctime>
#include "Framework/Types.hpp"

namespace bpf
{
    class Timer
    {
    private:
        clock_t _curtm;

    public:
        inline Timer()
            : _curtm(clock())
        {
        }

        /**
         * Returns the time in seconds since last call to Restart
         */
        inline float Restart()
        {
            clock_t cur = clock();
            float delta = static_cast<float>(cur - _curtm)
                / static_cast<float>(CLOCKS_PER_SEC);
            _curtm = cur;
            return (delta);
        }
    };
}
