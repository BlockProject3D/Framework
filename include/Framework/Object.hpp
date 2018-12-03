#ifndef OBJECT_H_
#define OBJECT_H_

namespace Framework
{
    class ENGINE_API FObject
    {
    private:
        Framework::FList<void **> Refs;
    public:
        virtual ~FObject();
        inline void AddRef(void **ref)
        {
            Refs.Add(ref);
        }
        inline void RemoveRef(void **ref)
        {
            Refs.Remove(ref);
        }
    };
    
    /**
     * The object smart pointer, allows you to automatically reset all instances of FObjectPtr when a given underlying instance is destroyed
     * This class only works with types extending the FObject class
     * @tparam T the type of the underlying instance
     */
    template <class T /* extends FObject */>
    class ENGINE_API FObjectPtr
    {
    private:
        T *RawPtr;
    public:
        inline FObjectPtr()
            : RawPtr(Null)
        {
        }
        inline ~FObjectPtr()
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
        inline bool operator==(const FObjectPtr<T> &other) const
        {
            return (RawPtr == other.RawPtr);
        }
        inline bool operator!=(const FObjectPtr<T> &other) const
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
        inline FObjectPtr<T> &operator=(T *other)
        {
            if (RawPtr != NULL)
                RawPtr->RemoveRef((void **)&RawPtr);
            RawPtr = other;
            if (RawPtr != NULL)
                RawPtr->AddRef((void **)&RawPtr);
            return (*this);
        }
        inline FObjectPtr<T> &operator=(const FObjectPtr<T> &other)
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

#endif /* !OBJECT_H_ */
