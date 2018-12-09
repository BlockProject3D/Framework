#pragma once

namespace bpf
{
    template<typename T>
    class BP_TPL_API WeakPtr;

    template <typename T>
    class BP_TPL_API SharedPtr
    {
    private:
        int *Count;
        int *WCount;
        T *RawPtr;

        inline SharedPtr(int *c, int *w, T *raw)
            : Count(c)
            , WCount(w)
            , RawPtr(raw)
        {
            if (Count != Null)
                ++*Count;
        }

    public:
        inline SharedPtr() noexcept
            : Count(Null)
            , WCount(Null)
            , RawPtr(Null)
        {
        }

        inline SharedPtr(T *raw)
            : Count(static_cast<int *>(Memory::Malloc(sizeof(int))))
            , WCount(static_cast<int *>(Memory::Malloc(sizeof(int))))
            , RawPtr(raw)
        {
            *Count = 1;
            *WCount = 0;
        }

        inline SharedPtr(SharedPtr<T> &&other) noexcept
            : Count(other.Count)
            , WCount(other.WCount)
            , RawPtr(other.RawPtr)
        {
            other.Count = Null;
            other.WCount = Null;
            other.RawPtr = Null;
        }

        template <typename T1>
        inline SharedPtr(const SharedPtr<T1> &other) noexcept
            : Count(other.Count)
            , WCount(other.WCount)
            , RawPtr(other.RawPtr)
        {
            if (Count != Null)
                ++*Count;
        }

        inline SharedPtr(const SharedPtr<T> &other) noexcept
            : Count(other.Count)
            , WCount(other.WCount)
            , RawPtr(other.RawPtr)
        {
            if (Count != Null)
                ++*Count;
        }

        ~SharedPtr();

        SharedPtr<T> &operator=(SharedPtr<T> &&other);

        SharedPtr<T> &operator=(const SharedPtr<T> &other);

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
        inline bool operator==(const SharedPtr<T1> &other) const noexcept
        {
            return (RawPtr == other.RawPtr);
        }
        template <typename T1>
        inline bool operator!=(const SharedPtr<T1> &other) const noexcept
        {
            return (RawPtr != other.RawPtr);
        }

        //Static casting
        //TODO : Throw class cast exception in debug build
        template <typename T1>
        inline SharedPtr<T1> StaticCast() const noexcept
        {
            return (SharedPtr<T1>(Count, WCount, static_cast<T1 *>(RawPtr)));
        }
        //End

        friend class WeakPtr<T>;

        template <typename T1>
        friend class SharedPtr;
    };
}
