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
#include "Framework/Memory/Memory.hpp"
#include "Framework/Map.hpp"
#include "Framework/List.hpp"
#include "Framework/String.hpp"
#include "Framework/JSON/JsonException.hpp"

namespace bpf
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
            Map<String, Json> _data;

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

            inline uint32 Size() const noexcept
            {
                return (_data.Size());
            }

            inline Map<String, Json>::Iterator Begin() const
            {
                return (_data.Begin());
            }

            inline Map<String, Json>::Iterator End() const
            {
                return (_data.End());
            }
        };

        class BPF_API Array
        {
        private:
            bpf::List<Json> _data;

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

            inline uint32 Size() const noexcept
            {
                return (_data.Size());
            }

            inline List<Json>::Iterator Begin() const
            {
                return (_data.Begin());
            }

            inline List<Json>::Iterator End() const
            {
                return (_data.End());
            }
        };

    private:
        EType _type;
        double _number;
        bool _bool;
        String _string;
        bpf::UniquePtr<Object> _object;
        bpf::UniquePtr<Array> _array;

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
            , _object(other._object != Null ? MakeUnique<Object>(**other._object) : Null)
            , _array(other._array != Null ? MakeUnique<Array>(**other._array) : Null)
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

        inline Json(const int val)
            : _type(EType::NUMBER)
            , _number((double)val)
            , _bool(false)
        {
        }

        inline Json(const uint32 val)
            : _type(EType::NUMBER)
            , _number((double)val)
            , _bool(false)
        {
        }

        inline Json(const double val)
            : _type(EType::NUMBER)
            , _number(val)
            , _bool(false)
        {
        }

        inline Json(const float val)
            : _type(EType::NUMBER)
            , _number((double)val)
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
            , _object(MakeUnique<Object>(val))
        {
        }

        inline Json(Object &&val)
            : _type(EType::OBJECT)
            , _number(0.0)
            , _bool(false)
            , _string(String::Empty)
            , _object(MakeUnique<Object>(std::move(val)))
        {
        }

        inline Json(const Array &arr)
            : _type(EType::ARRAY)
            , _number(0.0)
            , _bool(false)
            , _string(String::Empty)
            , _object(Null)
            , _array(MakeUnique<Array>(arr))
        {
        }

        inline Json(Array &&arr)
            : _type(EType::ARRAY)
            , _number(0.0)
            , _bool(false)
            , _string(String::Empty)
            , _object(Null)
            , _array(MakeUnique<Array>(std::move(arr)))
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
            _object = MakeUnique<Object>(val);
            return (*this);
        }

        inline Json &operator=(const Array &val)
        {
            _type = EType::ARRAY;
            _array = MakeUnique<Array>(val);
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

        inline explicit operator int() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((int)_number);
        }

        inline explicit operator uint32() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((uint32)_number);
        }

        inline explicit operator double() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return (_number);
        }

        inline explicit operator float() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((float)_number);
        }

        inline explicit operator bool() const
        {
            if (_type != EType::BOOLEAN)
                throw bpf::JsonException("Incompatible value type");
            return (_bool);
        }

        inline operator const String &() const
        {
            if (_type != EType::STRING)
                throw bpf::JsonException("Incompatible value type");
            return (_string);
        }

        inline operator const Array &() const
        {
            if (_type != EType::ARRAY)
                throw bpf::JsonException("Incompatible value type");
            return (**_array);
        }

        inline operator const Object &() const
        {
            if (_type != EType::OBJECT)
                throw bpf::JsonException("Incompatible value type");
            return (**_object);
        }

        inline int GetInt() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((int)_number);
        }

        inline uint32 GetUInt() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((uint32)_number);
        }

        inline double GetDouble() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return (_number);
        }

        inline float GetFloat() const
        {
            if (_type != EType::NUMBER)
                throw bpf::JsonException("Incompatible value type");
            return ((float)_number);
        }

        inline bool GetBool() const
        {
            if (_type != EType::BOOLEAN)
                throw bpf::JsonException("Incompatible value type");
            return (_bool);
        }

        inline const String &GetString() const
        {
            if (_type != EType::STRING)
                throw bpf::JsonException("Incompatible value type");
            return (_string);
        }

        inline const Array &GetArray() const
        {
            if (_type != EType::ARRAY)
                throw bpf::JsonException("Incompatible value type");
            return (**_array);
        }

        inline const Object &GetObject() const
        {
            if (_type != EType::OBJECT)
                throw bpf::JsonException("Incompatible value type");
            return (**_object);
        }
    };
}
