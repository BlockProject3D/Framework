#ifndef OBJECT_H_
#define OBJECT_H_

namespace bpf
{
    class BPF_API Object
    {
    private:
        List<void **> Refs;

        virtual ~Object();
        inline void AddRef(void **ref)
        {
            Refs.Add(ref);
        }
        inline void RemoveRef(void **ref)
        {
            Refs.Remove(ref);
        }
        
        template <class T>
        friend class ObjectPtr;
    };
}

#endif /* !OBJECT_H_ */
