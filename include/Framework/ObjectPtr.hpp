// Copyright (c) 2018, BlockProject
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
