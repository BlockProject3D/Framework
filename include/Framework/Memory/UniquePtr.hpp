#pragma once

namespace bpf
{
    template <typename T>
    class BPF_API UniquePtr
    {
    private:
        T *RawPtr;

    public:
        inline UniquePtr() noexcept
            : RawPtr(Null)
        {
        }

        inline UniquePtr(T *raw) noexcept
            : RawPtr(raw)
        {
        }

        inline UniquePtr(UniquePtr<T> &&other) noexcept
            : RawPtr(other.RawPtr)
        {
            other.RawPtr = Null;
        }

        template <typename T1>
        inline UniquePtr(UniquePtr<T1> &&other) noexcept
            : RawPtr(other.RawPtr)
        {
            other.RawPtr = Null;
        }

        inline ~UniquePtr()
        {
            Memory::Delete(RawPtr);
        }

        UniquePtr<T> &operator=(UniquePtr<T> &&other);

        inline T *operator*() const noexcept
        {
            return (RawPtr);
        }

        inline T *operator->() const noexcept
        {
            return (RawPtr);
        }

        inline bool operator==(const T *other) const noexcept
        {
            return (RawPtr == other);
        }

        inline bool operator!=(const T *other) const noexcept
        {
            return (RawPtr != other);
        }

        template <typename T1>
        inline bool operator==(const UniquePtr<T1> &other) const noexcept
        {
            return (RawPtr == other.RawPtr);
        }

        template <typename T1>
        inline bool operator!=(const UniquePtr<T1> &other) const noexcept
        {
            return (RawPtr != other.RawPtr);
        }

        template <typename T1>
        friend class UniquePtr;
    };
}
