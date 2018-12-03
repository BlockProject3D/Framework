#pragma once

namespace bpf
{
    /**
     * The object smart pointer, allows you to automatically reset all instances of FObjectPtr when a given underlying instance is destroyed
     * This class only works with types extending the FObject class
     * @tparam T the type of the underlying instance
     */
    template <class T /* extends FObject */>
    class BPF_API ObjectPtr
    {
    private:
        T *RawPtr;

    public:
        inline ObjectPtr()
            : RawPtr(Null)
        {
        }
        
        inline ~ObjectPtr()
        {
            if (RawPtr != NULL)
                RawPtr->RemoveRef((void **)&RawPtr);
        }
        
        inline bool operator==(T *other) const
        {
            return (RawPtr == other);
        }
        
        inline bool operator!=(T *other) const
        {
            return (RawPtr != other);
        }
        
        inline bool operator==(const ObjectPtr<T> &other) const
        {
            return (RawPtr == other.RawPtr);
        }
        
        inline bool operator!=(const ObjectPtr<T> &other) const
        {
            return (RawPtr != other.RawPtr);
        }
        
        inline T *operator->() const
        {
            return (RawPtr);
        }
        
        inline T *operator*() const
        {
            return (RawPtr);
        }
        
        ObjectPtr<T> &operator=(T *other)
        {
            if (RawPtr != NULL)
                RawPtr->RemoveRef((void **)&RawPtr);
            RawPtr = other;
            if (RawPtr != NULL)
                RawPtr->AddRef((void **)&RawPtr);
            return (*this);
        }
        
        ObjectPtr<T> &operator=(const ObjectPtr<T> &other)
        {
            if (RawPtr != NULL)
                RawPtr->RemoveRef((void **)&RawPtr);
            RawPtr = other.RawPtr;
            if (RawPtr != NULL)
                RawPtr->AddRef((void **)&RawPtr);
            return (*this);
        }
    };
}
