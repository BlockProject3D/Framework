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
#include "Framework/Memory/ClassCastException.hpp"
#include "Framework/Memory/MemUtils.hpp"
#include "Framework/TypeInfo.hpp"
#include <utility>

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
            virtual ~Storage()
            {
            }
            virtual Storage *Clone() = 0;
            virtual const char *GetTypeName() const noexcept = 0;
            virtual bool Equals(Storage *other) const = 0;
        };

        template <typename T>
        class DynamicStorage final : public Storage
        {
        private:
            T _data;
            const char *_tname;
            template <typename Q = T>
            inline typename std::enable_if<std::is_copy_constructible<Q>::value, Storage *>::type Useless()
            {
                return (memory::MemUtils::New<DynamicStorage<T>>(_data));
            }
            template <typename Q = T>
            inline typename std::enable_if<!std::is_copy_constructible<Q>::value, Storage *>::type Useless()
            {
                return (Null);
            }

        public:
            explicit inline DynamicStorage(const T &other)
                : Storage(TypeIndex<T>(), &_data)
                , _data(other)
                , _tname(TypeName<T>())
            {
            }
            inline const char *GetTypeName() const noexcept final
            {
                return (_tname);
            }
            explicit inline DynamicStorage(T &&other)
                : Storage(TypeIndex<T>(), &_data)
                , _data(std::move(other))
                , _tname(TypeName<T>())
            {
            }
            inline Storage *Clone() final
            {
                return (Useless());
            }
            inline bool Equals(Storage *other) const final
            {
                if (TypeId != other->TypeId)
                    return (false);
                return (_data == static_cast<DynamicStorage<T> *>(other)->_data);
            }
        };

        Storage *_storage;

    public:
        /**
         * Constructs a Null dynamic
         */
        inline Dynamic()
            : _storage(Null)
        {
        }

        /**
         * Copy constructor
         */
        inline Dynamic(const Dynamic &other)
            : _storage(other._storage == Null ? Null : other._storage->Clone())
        {
        }

        /**
         * Move constructor
         */
        inline Dynamic(Dynamic &&other) noexcept
            : _storage(other._storage)
        {
            other._storage = Null;
        }

        /**
         * Constructs a dynamic from an existing value
         * @tparam T the type of value to store
         * @param other the value to store
         */
        template <typename T>
        inline Dynamic(const T &other, typename std::enable_if<!std::is_same<T, Dynamic>::value>::type * = 0)
        {
            if (std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value)
                _storage = Null;
            else
                _storage = memory::MemUtils::New<DynamicStorage<T>>(other);
        }

        /**
         * Constructs a dynamic from an existing value
         * @tparam T the type of value to store
         * @param other the value to store
         */
        template <typename T>
        inline Dynamic(T &&other, typename std::enable_if<!std::is_same<T, Dynamic &>::value>::type * = 0)
        {
            if (std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value)
                _storage = Null;
            else
                _storage = memory::MemUtils::New<DynamicStorage<T>>(std::forward<T>(other));
        }

        ~Dynamic();

        /**
         * Returns the type identifier of the stored value
         * @return unsigned
         */
        inline fsize TypeId() const noexcept
        {
            return (_storage == Null ? 0 : _storage->TypeId);
        }

        /**
         * Copy assignment operator
         */
        Dynamic &operator=(const Dynamic &other);

        /**
         * Move assignment operator
         */
        Dynamic &operator=(Dynamic &&other) noexcept;

        /**
         * Assigns this dynamic to a different value
         * @tparam T the type of value to store
         * @param other the value to store
         * @return reference to this for chaining
         */
        template <typename T>
        inline Dynamic &operator=(const T &other)
        {
            if (std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value)
            {
                memory::MemUtils::Delete(_storage);
                _storage = Null;
                return (*this);
            }
            memory::MemUtils::Delete(_storage);
            _storage = memory::MemUtils::New<DynamicStorage<T>>(other);
            return (*this);
        }

        /**
         * Assigns this dynamic to a different value
         * @tparam T the type of value to store
         * @param other the value to store
         * @return reference to this for chaining
         */
        template <typename T>
        inline typename std::enable_if<!std::is_same<T, Dynamic &>::value, Dynamic &>::type operator=(T &&other)
        {
            if (std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value)
            {
                memory::MemUtils::Delete(_storage);
                _storage = Null;
                return (*this);
            }
            memory::MemUtils::Delete(_storage);
            _storage = memory::MemUtils::New<DynamicStorage<T>>(std::forward<T>(other));
            return (*this);
        }

        /**
         * Compare Dynamic
         * @param other operand
         * @return true if this equals other, false otherwise
         */
        bool operator==(const Dynamic &other) const;

        /**
         * Compare Dynamic
         * @param other operand
         * @return true if this doesn't equal other, false otherwise
         */
        bool operator!=(const Dynamic &other) const;

        /**
         * Converts this Dynamic to any explicit type
         * @tparam T type to convert to
         * @throw ClassCastException if type mismatch
         * @return mutable reference to T
         */
        template <typename T>
        explicit inline operator T &()
        {
            if (_storage == Null)
                throw memory::ClassCastException("Cannot cast null object");
            if (TypeIndex<T>() != TypeId())
                throw memory::ClassCastException(String("Cannot cast from ") + _storage->GetTypeName() + " to " +
                                                 TypeName<T>());
            return (*reinterpret_cast<T *>(_storage->DataPtr));
        }

        /**
         * Converts this Dynamic to any explicit type
         * @tparam T type to convert to
         * @throw ClassCastException if type mismatch
         * @return immutable reference to T
         */
        template <typename T>
        explicit inline operator const T &() const
        {
            if (_storage == Null)
                throw memory::ClassCastException("Cannot cast null object");
            if (TypeIndex<T>() != TypeId())
                throw memory::ClassCastException(String("Cannot cast from ") + _storage->GetTypeName() + " to " +
                                                 TypeName<T>());
            return (*reinterpret_cast<T *>(_storage->DataPtr));
        }
    };
}
