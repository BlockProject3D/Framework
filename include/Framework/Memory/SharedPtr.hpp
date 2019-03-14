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
    template<typename T>
    class BP_TPL_API WeakPtr;

    template <typename T>
    class BP_TPL_API SharedPtr
    {
    private:
        fint *Count;
        fint *WCount;
        T *RawPtr;

        inline SharedPtr(fint *c, fint *w, T *raw)
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

        explicit inline SharedPtr(T *raw)
            : Count(static_cast<fint *>(Memory::Malloc(sizeof(int))))
            , WCount(static_cast<fint *>(Memory::Malloc(sizeof(int))))
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
