#pragma once

namespace bpf
{
    template <typename T>
    class BPF_API WeakPtr
    {
    private:
        int *Count;
        int *WCount;
        T *RawPtr;

        inline WeakPtr(int *c, T *raw, int *w)
            : Count(c), WCount(w), RawPtr(raw)
        {
            if (Count != Null)
                ++*WCount;
        }

    public:
        inline WeakPtr(const SharedPtr<T> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count != Null)
                ++*WCount;
        }

        inline WeakPtr(const WeakPtr<T> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count != Null)
                ++*WCount;
        }

        ~WeakPtr();

        SharedPtr<T> Lock() noexcept;

        //Static casting
        //TODO : Throw class cast exception in debug build
        template <typename T1>
        inline WeakPtr<T1> StaticCast() const noexcept
        {
            return (WeakPtr<T1>(Count, static_cast<T1 *>(RawPtr), WCount));
        }
        //End

        template <typename T1>
        friend class WeakPtr;
    };
}
