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
        template <typename IType, typename T>
        class BP_TPL_API ArrayConstIteratorBase : public ConstIterator<IType, T>
        {
        protected:
            fsize _curid;
            fsize _max;
            const T *_arr;

        public:
            inline ArrayConstIteratorBase(const T *lowlevel, const fsize size, const fsize start)
                : _curid(start)
                , _max(size)
                , _arr(lowlevel)
            {
            }

            inline const T &operator*() const
            {
                return (_arr[_curid]);
            }

            inline const T *operator->() const
            {
                return (&_arr[_curid]);
            }

            inline bool operator==(const ArrayConstIteratorBase &other) const noexcept
            {
                return (_curid == other._curid);
            }

            inline bool operator!=(const ArrayConstIteratorBase &other) const noexcept
            {
                return (_curid != other._curid);
            }

            inline fsize Position() const noexcept
            {
                return (_curid);
            }
        };

        template <typename IType, typename T>
        class BP_TPL_API ArrayIteratorBase : Iterator<IType, T>
        {
        protected:
            fsize _curid;
            fsize _max;
            T *_arr;

        public:
            inline ArrayIteratorBase(T *lowlevel, const fsize size, const fsize start)
                : _curid(start)
                , _max(size)
                , _arr(lowlevel)
            {
            }

            inline const T &operator*() const
            {
                return (_arr[_curid]);
            }

            inline const T *operator->() const
            {
                return (&_arr[_curid]);
            }

            inline T &operator*()
            {
                return (_arr[_curid]);
            }

            inline T *operator->()
            {
                return (&_arr[_curid]);
            }

            inline bool operator==(const ArrayIteratorBase &other) const noexcept
            {
                return (_curid == other._curid);
            }

            inline bool operator!=(const ArrayIteratorBase &other) const noexcept
            {
                return (_curid != other._curid);
            }

            inline fsize Position() const noexcept
            {
                return (_curid);
            }
        };

        template <typename T>
        class BP_TPL_API ArrayConstIterator : public ArrayConstIteratorBase<ArrayConstIterator<T>, T>
        {
        private:
            using ArrayConstIteratorBase<ArrayConstIterator<T>,T>::_curid;
            using ArrayConstIteratorBase<ArrayConstIterator<T>,T>::_max;

        public:
            inline ArrayConstIterator(const T *lowlevel, const fsize size, const fsize start)
                : ArrayConstIteratorBase<ArrayConstIterator<T>,T>(lowlevel, size, start)
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
        };

        template <typename T>
        class BP_TPL_API ArrayConstReverseIterator : public ArrayConstIteratorBase<ArrayConstReverseIterator<T>,T>
        {
        private:
            using ArrayConstIteratorBase<ArrayConstReverseIterator<T>, T>::_curid;
            using ArrayConstIteratorBase<ArrayConstReverseIterator<T>, T>::_max;

        public:
            inline ArrayConstReverseIterator(const T *lowlevel, const fsize size, const fsize start)
                : ArrayConstIteratorBase<ArrayConstReverseIterator<T>, T>(lowlevel, size, start)
            {
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid - i) > _max ? (fsize)-1 : (_curid - i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid + i) >= _max ? (_max - 1) : (_curid + i);
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
        class BP_TPL_API ArrayIterator : public ArrayIteratorBase<ArrayIterator<T>, T>
        {
        private:
            using ArrayIteratorBase<ArrayIterator<T>, T>::_curid;
            using ArrayIteratorBase<ArrayIterator<T>, T>::_max;

        public:
            inline ArrayIterator(T *lowlevel, const fsize size, const fsize start)
                : ArrayIteratorBase<ArrayIterator<T>, T>(lowlevel, size, start)
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
        };

        template <typename T>
        class BP_TPL_API ArrayReverseIterator : public ArrayIteratorBase<ArrayIterator<T>, T>
        {
        private:
            using ArrayIteratorBase<ArrayIterator<T>, T>::_curid;
            using ArrayIteratorBase<ArrayIterator<T>, T>::_max;

        public:
            inline ArrayReverseIterator(T *lowlevel, const fsize size, const fsize start)
                : ArrayIteratorBase<ArrayIterator<T>, T>(lowlevel, size, start)
            {
            }

            inline void operator+=(fsize i)
            {
                _curid = (_curid - i) > _max ? (fsize)-1 : (_curid - i);
            }

            inline void operator-=(fsize i)
            {
                _curid = (_curid + i) >= _max ? (_max - 1) : (_curid + i);
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
