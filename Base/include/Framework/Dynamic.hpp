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
#include <utility>
#include "Framework/TypeInfo.hpp"
#include "Framework/Memory/MemUtils.hpp"
#include "Framework/Memory/ClassCastException.hpp"

namespace bpf
{
    /**
     * Cross platform dynamic variable type
     */
    class BPF_API Dynamic
    {
    private:
        class Storage
        {
        public:
            fsize TypeId;
            void *DataPtr;
            explicit inline Storage(const fsize type, void *dptr)
                : TypeId(type)
                , DataPtr(dptr)
            {
            }
            virtual ~Storage() {}
            virtual Storage *Clone() = 0;
            virtual const char *GetTypeName() const noexcept = 0;
        };

        template <typename T>
        class DynamicStorage final : public Storage, T
        {
        private:
            const char *_tname;

        public:
            explicit inline DynamicStorage(const T &other)
                : Storage(TypeIndex<T>(), static_cast<T *>(this))
                , T(other)
                , _tname(TypeName<T>())
            {
            }

            inline const char *GetTypeName() const noexcept final
            {
                return (_tname);
            }

            explicit inline DynamicStorage(T &&other)
                : Storage(TypeIndex<T>(), static_cast<T *>(this))
                , T(std::move(other))
                , _tname(TypeName<T>())
            {
            }

            inline Storage *Clone() final
            {
                return (memory::MemUtils::New<T>(*this));
            }
        };

        Storage *_storage;

        explicit inline Dynamic(Storage *storage)
            : _storage(storage)
        {
        }

    public:
        inline Dynamic()
            : _storage(Null)
        {
        }

        inline Dynamic(const Dynamic &other)
            : _storage(other._storage->Clone())
        {
        }

        inline Dynamic(Dynamic &&other) noexcept
            : _storage(other._storage)
        {
            other._storage = Null;
        }

        template <typename T>
        inline Dynamic(const T &other, typename std::enable_if<!std::is_same<T, Dynamic>::value>::type* = 0)
            : _storage(memory::MemUtils::New<DynamicStorage<T>>(other))
        {
        }

        template <typename T>
        inline Dynamic(T &&other, typename std::enable_if<!std::is_same<T, Dynamic>::value>::type* = 0)
            : _storage(memory::MemUtils::New<DynamicStorage<T>>(std::forward<T>(other)))
        {
        }

        inline ~Dynamic()
        {
            memory::MemUtils::Delete(_storage);
        }

        inline fsize TypeId() const noexcept
        {
            return (_storage == Null ? 0 : _storage->TypeId);
        }

        inline Dynamic &operator=(const Dynamic &other)
        {
            if (this == &other)
                return (*this);
            memory::MemUtils::Delete(_storage);
            _storage = other._storage == Null ? Null : other._storage->Clone();
            return (*this);
        }

        inline Dynamic &operator=(Dynamic &&other) noexcept
        {
            if (this == &other)
                return (*this);
            memory::MemUtils::Delete(_storage);
            _storage = other._storage;
            other._storage = Null;
            return (*this);
        }

        template <typename T>
        inline Dynamic &operator=(const T &other)
        {
            if (this == &other)
                return (*this);
            memory::MemUtils::Delete(_storage);
            _storage = memory::MemUtils::New<DynamicStorage<T>>(other);
            return (*this);
        }

        template <typename T>
        inline Dynamic &operator=(T &&other)
        {
            if (this == &other)
                return (*this);
            memory::MemUtils::Delete(_storage);
            _storage = memory::MemUtils::New<DynamicStorage<T>>(std::forward<T>(other));
            return (*this);
        }

        template <typename T>
        inline operator T &()
        {
            if (_storage == Null)
                throw memory::ClassCastException("Cannot cast null object");
            if (TypeIndex<T>() != TypeId())
                throw memory::ClassCastException(String("Cannot cast from ") + _storage->GetTypeName() + " to " + TypeName<T>());
            return (*reinterpret_cast<T *>(_storage->DataPtr));
        }

        template <typename T>
        inline operator const T &() const
        {
            if (_storage == Null)
                throw memory::ClassCastException("Cannot cast null object");
            if (TypeIndex<T>() != TypeId())
                throw memory::ClassCastException(String("Cannot cast from ") + _storage->GetTypeName() + " to " + TypeName<T>());
            return (*reinterpret_cast<T *>(_storage->DataPtr));
        }
    };
}
