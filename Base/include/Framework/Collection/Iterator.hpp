// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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
    namespace collection
    {
        /**
         * Abstract iterator
         * @tparam C the target collection type for this Iterator
         * @tparam T the target iterated object type
         */
        template <typename C, typename T>
        class BP_TPL_API IIterator
        {
        public:
            virtual ~IIterator() {}
            virtual IIterator &operator++() = 0;
            virtual IIterator &operator--() = 0;
            virtual const T &operator*() const = 0;
            virtual const T *operator->() const = 0;
            virtual bool operator==(const C &other) const = 0;
            virtual bool operator!=(const C &other) const = 0;
        };

        template <typename C>
        class BP_TPL_API ReverseAdapter
        {
        private:
            const C &_ref;

        public:
            inline explicit ReverseAdapter(const C &ref)
                : _ref(ref)
            {
            }
            inline typename C::ReverseIterator begin() const
            {
                return (_ref.rbegin());
            }
            inline typename C::ReverseIterator end() const
            {
                return (_ref.rend());
            }
        };

        /**
         * Utility function used to walk a iterable collection in reverse order using the C++ 11 range for loop syntax
         * @tparam C the collection type
         * @param container the actual collection to iterate in reverse order
         */
        template <typename C>
        BP_TPL_API ReverseAdapter<C> Reverse(const C &container)
        {
            return (ReverseAdapter<C>(container));
        }
    }
};
