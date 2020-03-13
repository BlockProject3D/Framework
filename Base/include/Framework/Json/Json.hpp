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
            enum class EType
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
                Object()
                {
                }
                Object(const std::initializer_list<std::pair<String, Json>> &lst);

                inline Json &operator[](const String &name)
                {
                    return (_data[name]);
                }

                inline const Json &operator[](const String &name) const
                {
                    return (_data[name]);
                }

                inline fsize Size() const noexcept
                {
                    return (_data.Size());
                }

                inline collection::Map<String, Json>::Iterator begin() const
                {
                    return (_data.begin());
                }

                inline collection::Map<String, Json>::Iterator end() const
                {
                    return (_data.end());
                }

                inline collection::Map<String, Json>::ReverseIterator rbegin() const
                {
                    return (_data.rbegin());
                }

                inline collection::Map<String, Json>::ReverseIterator rend() const
                {
                    return (_data.rend());
                }
            };

            class BPF_API Array
            {
            private:
                collection::List<Json> _data;

            public:
                Array()
                {
                }
                Array(const std::initializer_list<Json> &vals);

                inline void Add(const Json &json)
                {
                    _data.Add(json);
                }

                inline void Add(Json &&data)
                {
                    _data.Add(data);
                }

                inline void Remove(const Json &val)
                {
                    _data.Remove(val);
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

                inline collection::List<Json>::Iterator begin() const
                {
                    return (_data.begin());
                }

                inline collection::List<Json>::Iterator end() const
                {
                    return (_data.end());
                }

                inline collection::List<Json>::ReverseIterator rbegin() const
                {
                    return (_data.rbegin());
                }

                inline collection::List<Json>::ReverseIterator rend() const
                {
                    return (_data.rend());
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
                : _type(EType::STRING)
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

            inline Json &operator=(const int val)
            {
                _type = EType::NUMBER;
                _number = (double)val;
                return (*this);
            }

            inline Json &operator=(const uint32 val)
            {
                _type = EType::NUMBER;
                _number = (double)val;
                return (*this);
            }

            inline Json &operator=(const float val)
            {
                _type = EType::NUMBER;
                _number = (float)val;
                return (*this);
            }

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

            bool operator==(const Json &other) const;

            inline bool operator!=(const Json &other) const
            {
                return (!operator==(other));
            }

            inline explicit operator double() const
            {
                if (_type != EType::NUMBER)
                    throw JsonException("Incompatible value type");
                return (_number);
            }

            inline explicit operator bool() const
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

            inline double AsDouble() const
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
