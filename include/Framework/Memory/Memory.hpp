#pragma once
#include "Framework/Memory/MemoryException.hpp"
#ifdef BUILD_DEBUG
    #include <mutex>
#endif

#define Null nullptr

namespace bpf
{
    class BPF_API Memory
    {
#ifdef BUILD_DEBUG
    private:
        static size_t CurUsedMem;
        static int Allocs;
        static std::mutex MemMutex;
#endif
    public:
        static void *Malloc(size_t size);
        static void Free(void *addr);
        static void *Realloc(void *addr, size_t newsize);

        template <typename T, typename ...Args>
        inline static T *New(Args&&... args)
        {
            T *obj = static_cast<T *>(Malloc(sizeof(T)));

            new (obj) T(args...);
            return (obj);
        }

        template <typename T>
        inline static void Delete(T *obj)
        {
            if (obj == Null)
                return;
            obj->~T();
            Free(obj);
        }

#ifdef BUILD_DEBUG
        inline static int GetAllocCount() noexcept
        {
            return (Allocs);
        }
        inline static size_t GetUsedMem() noexcept
        {
            return (CurUsedMem);
        }
#endif
    };
};

#include "Framework/Memory/UniquePtr.hpp"
#include "Framework/Memory/SharedPtr.hpp"
#include "Framework/Memory/WeakPtr.hpp"
#include "Framework/Memory/Utility.hpp"

#include "Framework/Memory/SharedPtr.impl.hpp"
#include "Framework/Memory/WeakPtr.impl.hpp"
#include "Framework/Memory/UniquePtr.impl.hpp"
