#pragma once

namespace bpf
{
    class BPF_API Thread
    {
    private:
        void *_handle;
        bool _exit;

    public:
        Thread();
        ~Thread();

        void Start();
        void Kill(const bool force = false);

        inline bool ShouldExit() const noexcept
        {
            return (_exit);
        }

        virtual void Run() = 0;
    };
}
