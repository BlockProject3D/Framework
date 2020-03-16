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
#include "Framework/Collection/ArrayList.hpp"
#include "Framework/Collection/StackException.hpp"

namespace bpf
{
    namespace collection
    {
        template <typename T>
        class BP_TPL_API Stack
        {
        private:
            fsize _maxSize;
            ArrayList<T> _data;

        public:
            explicit Stack(const fsize maxsize = 0);
            Stack(const std::initializer_list<T> &lst);

            inline Stack(const Stack<T> &other)
                : _maxSize(other._maxSize)
                , _data(other._data)
            {
            }

            inline Stack(Stack<T> &&other)
                : _maxSize(other._maxSize)
                , _data(std::move(other._data))
            {
            }

            inline Stack<T> &operator=(const Stack<T> &other)
            {
                _maxSize = other._maxSize;
                _data = other._data;
                return (*this);
            }

            inline Stack<T> &operator=(Stack<T> &&other)
            {
                _maxSize = other._maxSize;
                _data = std::move(other._data);
                return (*this);
            }

            void Clear();

            /**
             * Pushes an element on the stack
             */
            void Push(const T &element);

            /**
             * Pushes an element on the stack
             */
            void Push(T &&element);

            /**
             * Extracts the top of the stack
             */
            T Pop();

            /**
             * Returns the top of the stack
             */
            inline T &Top()
            {
                return (_data.Last());
            }

            /**
             * Returns the top of the stack
             */
            inline const T &Top() const
            {
                return (_data.Last());
            }

            inline fsize Size() const
            {
                return (_data.Size());
            }
        };
    }
};

#include "Framework/Collection/Stack.impl.hpp"
