#ifndef MEMORY_H_
# define MEMORY_H_

# define Null nullptr

#ifdef BUILD_DEBUG
#   include <mutex>
#endif

namespace Framework
{
    class ENGINE_API FMemoryException final : public bpf::Exception
    {
    public:
        inline FMemoryException() {}
        inline const char *GetType() const
        {
            return ("MemoryException");
        }
    };

    class ENGINE_API FMemory
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
    
    template <typename T>
    class ENGINE_API FUniquePtr
    {
    private:
        T *RawPtr;
    public:
        inline FUniquePtr() noexcept
            : RawPtr(Null)
        {
        }
        inline FUniquePtr(T *raw) noexcept
            : RawPtr(raw)
        {
        }
        inline FUniquePtr(FUniquePtr<T> &&other) noexcept
            : RawPtr(other.RawPtr)
        {
            other.RawPtr = Null;
        }
        template <typename T1>
        inline FUniquePtr(FUniquePtr<T1> &&other) noexcept
            : RawPtr(other.RawPtr)
        {
            other.RawPtr = Null;
        }
        inline ~FUniquePtr()
        {
            FMemory::Delete(RawPtr);
        }
        inline FUniquePtr<T> &operator=(FUniquePtr<T> &&other)
        {
            if (RawPtr != Null)
                FMemory::Delete(RawPtr);
            RawPtr = other.RawPtr;
            other.RawPtr = Null;
            return (*this);
        }
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
        inline bool operator==(const FUniquePtr<T1> &other) const noexcept
        {
            return (RawPtr == other.RawPtr);
        }
        template <typename T1>
        inline bool operator!=(const FUniquePtr<T1> &other) const noexcept
        {
            return (RawPtr != other.RawPtr);
        }
        
        template <typename T1>
        friend class FUniquePtr;
    };
    
    template<typename T>
    class ENGINE_API FWeakPtr;

    template <typename T>
    class ENGINE_API FSharedPtr
    {
    private:
        int *Count;
        int *WCount;
        T *RawPtr;
        
        inline FSharedPtr(int *c, int *w, T *raw)
            : Count(c), WCount(w), RawPtr(raw)
        {
            if (Count == Null)
                return;
            ++*Count;
        }

    public:
        inline FSharedPtr() noexcept
            : Count(Null), WCount(Null), RawPtr(Null)
        {
        }
        inline FSharedPtr(T *raw)
            : Count(static_cast<int *>(FMemory::Malloc(sizeof(int)))), WCount(static_cast<int *>(FMemory::Malloc(sizeof(int)))), RawPtr(raw)
        {
            *Count = 1;
            *WCount = 0;
        }
        inline FSharedPtr(FSharedPtr<T> &&other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            other.Count = Null;
            other.WCount = Null;
            other.RawPtr = Null;
        }
        template <typename T1>
        inline FSharedPtr(const FSharedPtr<T1> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count == Null)
                return;
            ++*Count;
        }
        inline FSharedPtr(const FSharedPtr<T> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count == Null)
                return;
            ++*Count;
        }
        inline ~FSharedPtr()
        {
            if (Count == Null)
                return;
            --*Count;
            if (*Count <= 0)
            {
                FMemory::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    FMemory::Free(Count);
                    FMemory::Free(WCount);
                }
            }
        }
        inline FSharedPtr<T> &operator=(FSharedPtr<T> &&other)
        {
            if (Count != Null)
            {
                --*Count;
                if (*Count <= 0)
                {
                    FMemory::Delete(RawPtr);
                    if (*WCount <= 0)
                    {
                        FMemory::Free(Count);
                        FMemory::Free(WCount);
                    }
                }
            }
            Count = other.Count;
            WCount = other.WCount;
            RawPtr = other.RawPtr;
            other.Count = Null;
            other.WCount = Null;
            other.RawPtr = Null;
            return (*this);
        }
        inline FSharedPtr<T> &operator=(const FSharedPtr<T> &other)
        {
            if (Count != Null)
            {
                --*Count;
                if (*Count <= 0)
                {
                    FMemory::Delete(RawPtr);
                    if (*WCount <= 0)
                    {
                        FMemory::Free(Count);
                        FMemory::Free(WCount);
                    }
                }
            }
            Count = other.Count;
            WCount = other.WCount;
            RawPtr = other.RawPtr;
            if (Count != Null)
                ++*Count;
            return (*this);
        }
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
        inline bool operator==(const FSharedPtr<T1> &other) const noexcept
        {
            return (RawPtr == other.RawPtr);
        }
        template <typename T1>
        inline bool operator!=(const FSharedPtr<T1> &other) const noexcept
        {
            return (RawPtr != other.RawPtr);
        }

        //Static casting
        template <typename T1>
        inline FSharedPtr<T1> StaticCast() const noexcept
        {
            return (FSharedPtr<T1>(Count, WCount, static_cast<T1 *>(RawPtr)));
        }
        //End

        friend class FWeakPtr<T>;
        
        template <typename T1>
        friend class FSharedPtr;
    };
    
    template <typename T>
    class ENGINE_API FWeakPtr
    {
    private:
        int *Count;
        int *WCount;
        T *RawPtr;
        
        inline FWeakPtr(int *c, T *raw, int *w)
            : Count(c), WCount(w), RawPtr(raw)
        {
            if (Count == Null)
                return;
            ++*WCount;
        }
        
    public:
        inline FWeakPtr(const FSharedPtr<T> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count == Null)
                return;
            ++*WCount;
        }
        inline FWeakPtr(const FWeakPtr<T> &other) noexcept
            : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
        {
            if (Count == Null)
                return;
            ++*WCount;
        }
        inline ~FWeakPtr()
        {
            if (Count == Null)
                return;
            --*WCount;
            if (*WCount <= 0 && *Count <= 0)
            {
                FMemory::Free(WCount);
                FMemory::Free(Count);
            }
        }
        inline FSharedPtr<T> Lock() noexcept
        {
            if (Count == Null || *Count <= 0)
                return (FSharedPtr<T>());
            FSharedPtr<T> res;
            res.WCount = WCount;
            res.Count = Count;
            res.RawPtr = RawPtr;
            ++*res.Count;
            return (res);
        }
        
        //Static casting
        template <typename T1>
        inline FWeakPtr<T1> StaticCast() const noexcept
        {
            return (FWeakPtr<T1>(Count, static_cast<T1 *>(RawPtr), WCount));
        }
        //End

        template <typename T1>
        friend class FWeakPtr;
    };
    
    template <typename T, typename ...Args>
    class ENGINE_API FUnique
    {
    public:
        using type = FUniquePtr<T>;
        
        inline static type New(Args&&... args)
        {
            return (FUniquePtr<T>(FMemory::New<T>(args...)));
        }
    };
    
    template <typename T, typename ...Args>
    class ENGINE_API FShared
    {
    public:
        using type = FSharedPtr<T>;
        
        inline static type New(Args&&... args)
        {
            return (FSharedPtr<T>(FMemory::New<T>(args...)));
        }
    };
    
    template <typename T, typename ...Args>
    class ENGINE_API FRaw
    {
    public:
        using type = T *;
        
        inline static type New(Args&&... args)
        {
            return (FMemory::New<T>(args...));
        }
    };
    
    template <template <typename, typename...> class Manager, typename T, typename ...Args>
    inline typename Manager<T, Args...>::type NewObject(Args&&... args)
    {
        return (Manager<T, Args...>::New(std::forward<Args>(args)...));
    }
};

#endif /* !MEMORY_H_ */
