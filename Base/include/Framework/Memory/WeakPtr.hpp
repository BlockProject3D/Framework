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
    template <typename T>
    class BP_TPL_API WeakPtr
    {
    private:
        fint *Count;
        fint *WCount;
        T *RawPtr;

        inline WeakPtr(fint *c, T *raw, fint *w)
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