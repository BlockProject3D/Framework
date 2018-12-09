#pragma once

namespace bpf
{
    class BPF_API Mutex
    {
    private:
        void *_handle;

    public:
        Mutex();
        ~Mutex();

        /**
         * Locks this mutex
         */
        void Lock();

        /**
         * Unlocks this mutex
         */
        void Unlock();
    };
}
