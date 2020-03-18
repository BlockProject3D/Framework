// Copyright (c) 2020, BlockProject
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
#include "Framework/Memory/Utility.hpp"
#include "Framework/Collection/Map.hpp"
#include "Framework/Collection/List.hpp"
#include "Framework/String.hpp"
#include "Framework/Json/JsonException.hpp"

namespace bpf
{
    namespace json
    {
        class BPF_API Json
        {
        public:
            enum EType
            {
                NUMBER,
                BOOLEAN,
                STRING,
                ARRAY,
                OBJECT,
                NONE
            };

            class BPF_API Object
            {
            private:
                collection::Map<String, Json> _data;

            public:
                using Iterator = collection::Map<String, Json>::Iterator;
                using ReverseIterator = collection::Map<String, Json>::ReverseIterator;

                Object()
                {
                }
                explicit Object(const std::initializer_list<std::pair<String, Json>> &lst);
                explicit inline Object(const collection::Map<String, Json> &map)
                    : _data(map)
                {
                }
                explicit inline Object(collection::Map<String, Json> &&map)
                    : _data(std::move(map))
                {
                }

                inline Json &operator[](const String &name)
                {
                    return (_data[name]);
                }

                inline const Json &operator[](const String &name) const
                {
                    return (_data[name]);
                }

                inline void Add(const String &name, const Json &json)
                {
                    _data.Add(name, json);
                }

                inline void Add(const String &name, Json &&data)
                {
                    _data.Add(name, std::move(data));
                }

                inline void RemoveAt(const String &name)
                {
                    _data.RemoveAt(name);
                }

                inline fsize Size() const noexcept
                {
                    return (_data.Size());
                }

                inline Iterator begin() const
                {
                    return (_data.begin());
                }

                inline Iterator end() const
                {
                    return (_data.end());
                }

                inline ReverseIterator rbegin() const
                {
                    return (_data.rbegin());
                }

                inline ReverseIterator rend() const
                {
                    return (_data.rend());
                }

                inline const collection::Map<String, Json> &Properties() const noexcept
                {
                    return (_data);
                }

                inline collection::Map<String, Json> &Properties() noexcept
                {
                    return (_data);
                }
            };

            class BPF_API Array
            {
            private:
                collection::List<Json> _data;

            public:
                using Iterator = collection::List<Json>::Iterator;
                using ReverseIterator = collection::List<Json>::ReverseIterator;

                Array()
                {
                }
                explicit Array(const std::initializer_list<Json> &vals);
                explicit inline Array(const collection::List<Json> &vals)
                    : _data(vals)
                {
                }
                explicit inline Array(collection::List<Json> &&vals)
                    : _data(std::move(vals))
                {
                }

                inline void Add(const Json &json)
                {
                    _data.Add(json);
                }

                inline void Add(Json &&data)
                {
                    _data.Add(std::move(data));
                }

                inline void RemoveAt(const uint32 id)
                {
                    _data.RemoveAt(id);
                }

                inline Json &operator[](const uint32 id)
                {
                    return (_data[id]);
                }

                inline const Json &operator[](const uint32 id) const
                {
                    return (_data[id]);
                }

                inline fsize Size() const noexcept
                {
                    return (_data.Size());
                }

                inline Iterator begin() const
                {
                    return (_data.begin());
                }

                inline Iterator end() const
                {
                    return (_data.end());
                }

                inline ReverseIterator rbegin() const
                {
                    return (_data.rbegin());
                }

                inline ReverseIterator rend() const
                {
                    return (_data.rend());
                }

                inline const collection::List<Json> &Items() const noexcept
                {
                    return (_data);
                }

                inline collection::List<Json> &Items() noexcept
                {
                    return (_data);
                }
            };

        private:
            EType _type;
            double _number;
            bool _bool;
            String _string;
            memory::UniquePtr<Object> _object;
            memory::UniquePtr<Array> _array;

        public:
            inline Json()
                : _type(EType::NONE)
                , _number(0.0)
                , _bool(false)
            {
            }

            inline Json(const Json &other)
                : _type(other._type)
                , _number(other._number)
                , _bool(other._bool)
                , _string(other._string)
                , _object(other._object != Null ? memory::MakeUnique<Object>(*other._object) : Null)
                , _array(other._array != Null ? memory::MakeUnique<Array>(*other._array) : Null)
            {
            }

            inline Json(Json &&other)
                : _type(other._type)
                , _number(other._number)
                , _bool(other._bool)
                , _string(std::move(other._string))
                , _object(std::move(other._object))
                , _array(std::move(other._array))
            {
            }

            inline Json(const double val)
                : _type(EType::NUMBER)
                , _number(val)
                , _bool(false)
            {
            }

            inline Json(const bool val)
                : _type(EType::BOOLEAN)
                , _number(0.0)
                , _bool(val)
            {
            }

            inline Json(const String &val)
                : _type(EType::STRING)
                , _number(0.0)
                , _bool(false)
                , _string(val)
            {
            }

            inline Json(const char *val)
                : _type(val != Null ? EType::STRING : EType::NONE)
                , _number(0.0)
                , _bool(false)
                , _string(val)
            {
            }

            inline Json(String &&val)
                : _type(EType::STRING)
                , _number(0.0)
                , _bool(false)
                , _string(std::move(val))
            {
            }

            inline Json(const Object &val)
                : _type(EType::OBJECT)
                , _number(0.0)
                , _bool(false)
                , _string(String::Empty)
                , _object(memory::MakeUnique<Object>(val))
            {
            }

            inline Json(Object &&val)
                : _type(EType::OBJECT)
                , _number(0.0)
                , _bool(false)
                , _string(String::Empty)
                , _object(memory::MakeUnique<Object>(std::move(val)))
            {
            }

            inline Json(const Array &arr)
                : _type(EType::ARRAY)
                , _number(0.0)
                , _bool(false)
                , _string(String::Empty)
                , _object(Null)
                , _array(memory::MakeUnique<Array>(arr))
            {
            }

            inline Json(Array &&arr)
                : _type(EType::ARRAY)
                , _number(0.0)
                , _bool(false)
                , _string(String::Empty)
                , _object(Null)
                , _array(memory::MakeUnique<Array>(std::move(arr)))
            {
            }

            Json &operator=(const Json &other);

            Json &operator=(Json &&other);

            inline Json &operator=(const double val)
            {
                _type = EType::NUMBER;
                _number = val;
                return (*this);
            }

            inline Json &operator=(const bool val)
            {
                _type = EType::BOOLEAN;
                _bool = val;
                return (*this);
            }

            inline Json &operator=(const String &val)
            {
                _type = EType::STRING;
                _string = val;
                return (*this);
            }

            inline Json &operator=(const char *val)
            {
                _type = EType::STRING;
                _string = val;
                return (*this);
            }

            inline Json &operator=(const Object &val)
            {
                _type = EType::OBJECT;
                _object = memory::MakeUnique<Object>(val);
                return (*this);
            }

            inline Json &operator=(const Array &val)
            {
                _type = EType::ARRAY;
                _array = memory::MakeUnique<Array>(val);
                return (*this);
            }

            inline EType Type() const noexcept
            {
                return (_type);
            }

            inline bool operator==(double other) const
            {
                if (_type != EType::NUMBER)
                    return (false);
                return (_number == other);
            }

            inline bool operator==(bool other) const
            {
                if (_type != EType::BOOLEAN)
                    return (false);
                return (_bool == other);
            }

            inline bool operator==(const String &other) const
            {
                if (_type != EType::STRING)
                    return (false);
                return (_string == other);
            }

            inline bool operator==(const char *other) const
            {
                if (_type == EType::NONE && other == Null)
                    return (true);
                if (_type != EType::STRING)
                    return (false);
                return (_string == String(other));
            }

            inline operator double() const
            {
                if (_type != EType::NUMBER)
                    throw JsonException("Incompatible value type");
                return (_number);
            }

            inline operator bool() const
            {
                if (_type != EType::BOOLEAN)
                    throw JsonException("Incompatible value type");
                return (_bool);
            }

            inline operator const String &() const
            {
                if (_type != EType::STRING)
                    throw JsonException("Incompatible value type");
                return (_string);
            }

            inline operator const Array &() const
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            inline operator const Object &() const
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }

            inline operator Array &()
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            inline operator Object &()
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }

            inline double AsNumber() const
            {
                if (_type != EType::NUMBER)
                    throw JsonException("Incompatible value type");
                return (_number);
            }

            inline bool AsBool() const
            {
                if (_type != EType::BOOLEAN)
                    throw JsonException("Incompatible value type");
                return (_bool);
            }

            inline const String &AsString() const
            {
                if (_type != EType::STRING)
                    throw JsonException("Incompatible value type");
                return (_string);
            }

            inline const Array &AsArray() const
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            inline const Object &AsObject() const
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }
        };
    }
}
