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
#include "Framework/Collection/Iterator.hpp"

namespace bpf
{
    namespace collection
    {
        template <typename T>
        class BP_TPL_API ArrayConstIterator : public ConstIterator<ArrayConstIterator<T>, T>
        {
        protected:
            fsize _curid;
            fsize _max;
            const T *_arr;

        public:
            inline ArrayConstIterator(const T *lowlevel, const fsize size, const fsize start)
                : _curid(start)
                , _max(size)
                , _arr(lowlevel)
            {
            }

            inline ArrayConstIterator &operator++()
            {
                if (_curid != _max)
                    _curid++;
                return (*this);
            }

            inline ArrayConstIterator &operator--()
            {
                if (_curid > 0)
                    _curid--;
                return (*this);
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid + i) > _max ? _max : (_curid + i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid - i) > _max ? 0 : (_curid - i);
            }

            inline const T &operator*() const
            {
                return (_arr[_curid]);
            }

            inline const T *operator->() const
            {
                return (&_arr[_curid]);
            }

            inline bool operator==(const ArrayConstIterator &other) const noexcept
            {
                return (_curid == other._curid);
            }

            inline bool operator!=(const ArrayConstIterator &other) const noexcept
            {
                return (_curid != other._curid);
            }

            inline fsize Position() const noexcept
            {
                return (_curid);
            }
        };

        template <typename T>
        class BP_TPL_API ArrayConstReverseIterator : public ArrayConstIterator<T>
        {
        private:
            using ArrayConstIterator<T>::_curid;
            using ArrayConstIterator<T>::_max;

        public:
            inline ArrayConstReverseIterator(const T *lowlevel, const fsize size, const fsize start)
                : ArrayConstIterator<T>(lowlevel, size, start)
            {
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid - i) > _max ? 0 : (_curid - i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid + i) > _max ? _max : (_curid + i);
            }

            inline ArrayConstReverseIterator &operator++()
            {
                if (_curid != (fsize)-1)
                    _curid--;
                return (*this);
            }

            inline ArrayConstReverseIterator &operator--()
            {
                if (_curid < _max - 1)
                    _curid++;
                return (*this);
            }
        };

        template <typename T>
        class BP_TPL_API ArrayIterator : public Iterator<ArrayIterator<T>, T>
        {
        protected:
            fsize _curid;
            fsize _max;
            T *_arr;

        public:
            inline ArrayIterator(T *lowlevel, const fsize size, const fsize start)
                : _curid(start)
                , _max(size)
                , _arr(lowlevel)
            {
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid + i) > _max ? _max : (_curid + i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid - i) > _max ? 0 : (_curid - i);
            }

            inline ArrayIterator &operator++()
            {
                if (_curid != _max)
                    _curid++;
                return (*this);
            }

            inline ArrayIterator &operator--()
            {
                if (_curid > 0)
                    _curid--;
                return (*this);
            }

            inline T &operator*()
            {
                return (_arr[_curid]);
            }

            inline T *operator->()
            {
                return (&_arr[_curid]);
            }

            inline bool operator==(const ArrayIterator &other) const noexcept
            {
                return (_curid == other._curid);
            }

            inline bool operator!=(const ArrayIterator &other) const noexcept
            {
                return (_curid != other._curid);
            }

            inline fsize Position() const noexcept
            {
                return (_curid);
            }
        };

        template <typename T>
        class BP_TPL_API ArrayReverseIterator : public ArrayIterator<T>
        {
        private:
            using ArrayIterator<T>::_curid;
            using ArrayIterator<T>::_max;

        public:
            inline ArrayReverseIterator(T *lowlevel, const fsize size, const fsize start)
                : ArrayIterator<T>(lowlevel, size, start)
            {
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid - i) > _max ? 0 : (_curid - i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid + i) > _max ? _max : (_curid + i);
            }

            inline ArrayReverseIterator &operator++()
            {
                if (_curid != (fsize)-1)
                    _curid--;
                return (*this);
            }

            inline ArrayReverseIterator &operator--()
            {
                if (_curid < _max - 1)
                    _curid++;
                return (*this);
            }
        };
    }
}
