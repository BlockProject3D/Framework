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
#include "Framework/Types.hpp"

namespace bpf
{
    namespace collection
    {
        /**
         * Constant iterator
         * @tparam IType the actual iterator type
         * @tparam T the target iterated value type
         */
        template <typename IType, typename T>
        class BP_TPL_API ConstIterator
        {
        public:
            /**
             * Advances this iterator forward
             * @param i how many steps should we advance
             */
            inline void operator+=(fsize i)
            {
                while (i > 0)
                {
                    static_cast<IType *>(this)->operator++();
                    --i;
                }
            }

            /**
             * Advances this iterator backward
             * @param i how many steps should we advance
             */
            inline void operator-=(fsize i)
            {
                while (i > 0)
                {
                    static_cast<IType *>(this)->operator--();
                    --i;
                }
            }

            /**
             * Increments this iterator to the next position
             * @return reference to iterator type
             */
            IType &operator++();

            /**
             * Decrements this iterator to the previous position
             * @return reference to iterator type
             */
            IType &operator--();

            /**
             * Access to the value type at the current iterator position
             * @return immutable reference to T
             */
            const T &operator*() const;

            /**
             * Access to the value type at the current iterator position
             * @return immutable pointer to T
             */
            const T *operator->() const;

            /**
             * Compare ConstIterator
             * @param other operand
             * @return true if this equals other, false otherwise
             */
            bool operator==(const IType &other) const noexcept;

            /**
             * Compare ConstIterator
             * @param other operand
             * @return true if this does not equal other, false otherwise
             */
            bool operator!=(const IType &other) const noexcept;
        };

        /**
         * Non-constant iterator
         * @tparam IType the actual iterator type
         * @tparam T the target iterated value type
         */
        template <typename IType, typename T>
        class BP_TPL_API Iterator
        {
        public:
            /**
             * Advances this iterator forward
             * @param i how many steps should we advance
             */
            void operator+=(fsize i)
            {
                while (i > 0)
                {
                    static_cast<IType *>(this)->operator++();
                    --i;
                }
            }

            /**
             * Advances this iterator backward
             * @param i how many steps should we advance
             */
            inline void operator-=(fsize i)
            {
                while (i > 0)
                {
                    static_cast<IType *>(this)->operator--();
                    --i;
                }
            }

            /**
             * Increments this iterator to the next position
             * @return reference to iterator type
             */
            IType &operator++();

            /**
             * Decrements this iterator to the previous position
             * @return reference to iterator type
             */
            IType &operator--();

            /**
             * Access to the value type at the current iterator position
             * @return mutable reference to T
             */
            T &operator*();

            /**
             * Access to the value type at the current iterator position
             * @return mutable pointer to T
             */
            T *operator->();

            /**
             * Access to the value type at the current iterator position
             * @return imutable reference to T
             */
            const T &operator*() const;

            /**
             * Access to the value type at the current iterator position
             * @return mutable pointer to T
             */
            const T *operator->() const;

            /**
             * Compare Iterator
             * @param other operand
             * @return true if this equals other, false otherwise
             */
            bool operator==(const IType &other) const noexcept;

            /**
             * Compare Iterator
             * @param other operand
             * @return true if this does not equal other, false otherwise
             */
            bool operator!=(const IType &other) const noexcept;
        };

        template <typename C>
        class BP_TPL_API ConstReverseAdapter
        {
        private:
            const C &_ref;

        public:
            inline explicit ConstReverseAdapter(const C &ref)
                : _ref(ref)
            {
            }
            inline typename C::CReverseIterator begin() const
            {
                return (_ref.rbegin());
            }
            inline typename C::CReverseIterator end() const
            {
                return (_ref.rend());
            }
        };

        template <typename C>
        class BP_TPL_API ReverseAdapter
        {
        private:
            C &_ref;

        public:
            inline explicit ReverseAdapter(C &ref)
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
        BP_TPL_API ConstReverseAdapter<C> Reverse(const C &container)
        {
            return (ConstReverseAdapter<C>(container));
        }

        /**
         * Utility function used to walk a iterable collection in reverse order using the C++ 11 range for loop syntax
         * @tparam C the collection type
         * @param container the actual collection to iterate in reverse order
         */
        template <typename C>
        BP_TPL_API ReverseAdapter<C> Reverse(C &container)
        {
            return (ReverseAdapter<C>(container));
        }
    }
}
