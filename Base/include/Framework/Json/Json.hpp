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
#include "Framework/Collection/List.hpp"
#include "Framework/Collection/Map.hpp"
#include "Framework/Json/JsonException.hpp"
#include "Framework/Memory/Utility.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace json
    {
        /**
         * Represents an arbitary Json value
         */
        class BPF_API Json
        {
        public:
            /**
             * Enumeration to represent the type of a Json value
             */
            enum EType
            {
                /**
                 * Indicates a Json number that stores doubles
                 */
                DOUBLE,

                /**
                 * Indicates a Json number that stores integers
                 */
                INTEGER,

                /**
                 * Indicates a Json boolean
                 */
                BOOLEAN,

                /**
                 * Indicates a Json string
                 */
                STRING,

                /**
                 * Indicates a Json array
                 */
                ARRAY,

                /**
                 * Indicates a Json object
                 */
                OBJECT,

                /**
                 * Indicates Json null
                 */
                NONE
            };

            class BPF_API Type
            {
            private:
                EType _internal;

                explicit inline Type(EType t) noexcept
                    : _internal(t)
                {
                }

            public:
                /**
                 * Checks if this Json Type is a number
                 * @return true if this is either a double or an integer, false otherwise
                 */
                inline bool IsNumber() const noexcept
                {
                    return (_internal == DOUBLE || _internal == INTEGER);
                }

                /**
                 * Checks if this Json Type is a number
                 * @return true if this is a double, false otherwise
                 */
                inline bool IsDouble() const noexcept
                {
                    return (_internal == DOUBLE);
                }

                /**
                 * Checks if this Json Type is a number
                 * @return true if this is an integer, false otherwise
                 */
                inline bool IsInteger() const noexcept
                {
                    return (_internal == INTEGER);
                }

                /**
                 * Checks if this Json Type is an object
                 * @return true if this is an object, false otherwise
                 */
                inline bool IsObject() const noexcept
                {
                    return (_internal == OBJECT);
                }

                /**
                 * Checks if this Json Type is an array
                 * @return true if this is an array, false otherwise
                 */
                inline bool IsArray() const noexcept
                {
                    return (_internal == ARRAY);
                }

                /**
                 * Checks if this Json Type is a string
                 * @return true if this is a string, false otherwise
                 */
                inline bool IsString() const noexcept
                {
                    return (_internal == STRING);
                }

                /**
                 * Checks if this Json Type is null
                 * @return true if this is null, false otherwise
                 */
                inline bool IsNull() const noexcept
                {
                    return (_internal == NONE);
                }

                /**
                 * Conversion operator
                 * @return internal EType
                 */
                inline operator EType() const noexcept
                {
                    return (_internal);
                }

                friend class Json;
            };

        private:
            using _bpf_internal_json_type = Type;

        public:
            /**
             * Represents a Json Object
             */
            class BPF_API Object
            {
            public:
                using Iterator = collection::Map<String, Json>::Iterator;
                using ReverseIterator = collection::Map<String, Json>::ReverseIterator;

                /**
                 * Object properties
                 */
                collection::Map<String, Json> Properties;

                /**
                 * Constructs an empty object
                 */
                Object()
                {
                }

                /**
                 * Constructs an Object from an existing initializer list
                 * @param lst the initial list of key-value pairs to add to this new Object
                 */
                explicit Object(const std::initializer_list<std::pair<String, Json>> &lst);

                /**
                 * Constructs an Object from an existing Map
                 * @param map the map to construct from
                 */
                explicit inline Object(const collection::Map<String, Json> &map)
                    : Properties(map)
                {
                }

                /**
                 * Constructs an Object from an existing Map
                 * @param map the map to construct from
                 */
                explicit inline Object(collection::Map<String, Json> &&map)
                    : Properties(std::move(map))
                {
                }

                /**
                 * Returns an element non-const mode
                 * @param name the property name
                 * @throw IndexException if key is not in this map
                 * @return mutable item
                 */
                inline Json &operator[](const String &name)
                {
                    return (Properties[name]);
                }

                /**
                 * Returns an element const mode
                 * @param name the property name
                 * @throw IndexException if key is not in this map
                 * @return immutable item
                 */
                inline const Json &operator[](const String &name) const
                {
                    return (Properties[name]);
                }

                /**
                 * Adds a new property in this object, replaces if the property already exists
                 * @param name the property name
                 * @param json the value to insert
                 */
                inline void Add(const String &name, const Json &json)
                {
                    Properties.Add(name, json);
                }

                /**
                 * Adds a new property in this object, replaces if the property already exists
                 * @param name the property name
                 * @param json the value to insert
                 */
                inline void Add(const String &name, Json &&json)
                {
                    Properties.Add(name, std::move(json));
                }

                /**
                 * Removes a property from this object
                 * @param name the property name
                 */
                inline void RemoveAt(const String &name)
                {
                    Properties.RemoveAt(name);
                }

                /**
                 * Returns the number of properties in this object
                 * @return number of properties as unsigned
                 */
                inline fsize Size() const noexcept
                {
                    return (Properties.Size());
                }

                /**
                 * Returns an iterator to the begining of the collection
                 * @return new iterator
                 */
                inline Iterator begin() const
                {
                    return (Properties.begin());
                }

                /**
                 * Returns an iterator to the end of the collection
                 * @return new iterator
                 */
                inline Iterator end() const
                {
                    return (Properties.end());
                }

                /**
                 * Returns a reverse iterator to the begining of the collection
                 * @return new iterator
                 */
                inline ReverseIterator rbegin() const
                {
                    return (Properties.rbegin());
                }

                /**
                 * Returns a reverse iterator to the end of the collection
                 * @return new iterator
                 */
                inline ReverseIterator rend() const
                {
                    return (Properties.rend());
                }
            };

            /**
             * Represents a Json Array
             */
            class BPF_API Array
            {
            public:
                using Iterator = collection::List<Json>::Iterator;
                using ReverseIterator = collection::List<Json>::ReverseIterator;

                /**
                 * Array content
                 */
                collection::List<Json> Items;

                /**
                 * Constructs an empty array
                 */
                Array()
                {
                }

                /**
                 * Constructs an Array from an existing initializer list
                 * @param vals the initial list of items to add to this new Array
                 */
                explicit Array(const std::initializer_list<Json> &vals);

                /**
                 * Constructs an Array from an existing List
                 * @param vals the list to construct from
                 */
                explicit inline Array(const collection::List<Json> &vals)
                    : Items(vals)
                {
                }

                /**
                 * Constructs an Array from an existing List
                 * @param vals the list to construct from
                 */
                explicit inline Array(collection::List<Json> &&vals)
                    : Items(std::move(vals))
                {
                }

                /**
                 * Adds a new item at the end of the array
                 * @param json the new value to append
                 */
                inline void Add(const Json &json)
                {
                    Items.Add(json);
                }

                /**
                 * Adds a new item at the end of the array
                 * @param data the new value to append
                 */
                inline void Add(Json &&data)
                {
                    Items.Add(std::move(data));
                }

                /**
                 * Remove an item at a given index
                 * @param id the index of the item to remove
                 */
                inline void RemoveAt(const fsize id)
                {
                    Items.RemoveAt(id);
                }

                /**
                 * Returns an element non-const mode
                 * @param id the index of the element, in case of out of bounds, throws
                 * @throw IndexException if id is out of bounds
                 * @return mutable item at index id
                 */
                inline Json &operator[](const fsize id)
                {
                    return (Items[id]);
                }

                /**
                 * Returns an element const mode
                 * @param id the index of the element, in case of out of bounds, throws
                 * @throw IndexException if id is out of bounds
                 * @return immutable item at index id
                 */
                inline const Json &operator[](const fsize id) const
                {
                    return (Items[id]);
                }

                /**
                 * Returns the number of items in this list
                 * @return number of items as unsigned
                 */
                inline fsize Size() const noexcept
                {
                    return (Items.Size());
                }

                /**
                 * Returns an iterator to the begining of the collection
                 * @return new iterator
                 */
                inline Iterator begin() const
                {
                    return (Items.begin());
                }

                /**
                 * Returns an iterator to the end of the collection
                 * @return new iterator
                 */
                inline Iterator end() const
                {
                    return (Items.end());
                }

                /**
                 * Returns a reverse iterator to the begining of the collection
                 * @return new iterator
                 */
                inline ReverseIterator rbegin() const
                {
                    return (Items.rbegin());
                }

                /**
                 * Returns a reverse iterator to the end of the collection
                 * @return new iterator
                 */
                inline ReverseIterator rend() const
                {
                    return (Items.rend());
                }
            };

        private:
            EType _type;
            double _double;
            int64 _integer;
            bool _bool;
            String _string;
            memory::UniquePtr<Object> _object;
            memory::UniquePtr<Array> _array;

        public:
            /**
             * Constructs a null Json value
             */
            inline Json()
                : _type(EType::NONE)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
            {
            }

            /**
             * Copy constructor
             */
            inline Json(const Json &other)
                : _type(other._type)
                , _double(other._double)
                , _integer(other._integer)
                , _bool(other._bool)
                , _string(other._string)
                , _object(other._object != nullptr ? memory::MakeUnique<Object>(*other._object) : nullptr)
                , _array(other._array != nullptr ? memory::MakeUnique<Array>(*other._array) : nullptr)
            {
            }

            /**
             * Move constructor
             */
            inline Json(Json &&other) noexcept
                : _type(other._type)
                , _double(other._double)
                , _integer(other._integer)
                , _bool(other._bool)
                , _string(std::move(other._string))
                , _object(std::move(other._object))
                , _array(std::move(other._array))
            {
            }

            /**
             * Constructs a Json number value
             * @param val the number to store
             */
            inline Json(const double val)
                : _type(EType::DOUBLE)
                , _double(val)
                , _integer((int64)val)
                , _bool(false)
            {
            }

            /**
             * Constructs a Json number value
             * @param val the number to store
             */
            inline Json(const int64 val)
                : _type(EType::INTEGER)
                , _double((double)val)
                , _integer(val)
                , _bool(false)
            {
            }

            /**
             * Constructs a Json boolean value
             * @param val the boolean to store
             */
            inline Json(const bool val)
                : _type(EType::BOOLEAN)
                , _double(0.0)
                , _integer(0)
                , _bool(val)
            {
            }

            /**
             * Constructs a Json string value
             * @param val the string to store
             */
            inline Json(const String &val)
                : _type(EType::STRING)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string(val)
            {
            }

            /**
             * Constructs a Json string value
             * @param val the string to store
             */
            inline Json(const char *val)
                : _type(val != nullptr ? EType::STRING : EType::NONE)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string(val)
            {
            }

            /**
             * Constructs a Json string value
             * @param val the string to store
             */
            inline Json(String &&val)
                : _type(EType::STRING)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string(std::move(val))
            {
            }

            /**
             * Constructs a Json object value
             * @param val the object to store
             */
            inline Json(const Object &val)
                : _type(EType::OBJECT)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string("")
                , _object(memory::MakeUnique<Object>(val))
            {
            }

            /**
             * Constructs a Json object value
             * @param val the object to store
             */
            inline Json(Object &&val)
                : _type(EType::OBJECT)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string("")
                , _object(memory::MakeUnique<Object>(std::move(val)))
            {
            }

            /**
             * Constructs a Json array value
             * @param arr the array to store
             */
            inline Json(const Array &arr)
                : _type(EType::ARRAY)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string("")
                , _object(nullptr)
                , _array(memory::MakeUnique<Array>(arr))
            {
            }

            /**
             * Constructs a Json array value
             * @param arr the array to store
             */
            inline Json(Array &&arr)
                : _type(EType::ARRAY)
                , _double(0.0)
                , _integer(0)
                , _bool(false)
                , _string("")
                , _object(nullptr)
                , _array(memory::MakeUnique<Array>(std::move(arr)))
            {
            }

            /**
             * Copy assignment operator
             */
            Json &operator=(const Json &other);

            /**
             * Move assignment operator
             */
            Json &operator=(Json &&other) noexcept;

            /**
             * Assigns this Json value to a number
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const double val)
            {
                _type = EType::DOUBLE;
                _double = val;
                _integer = (int64)val;
                return (*this);
            }

            /**
             * Assigns this Json value to a number
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const int64 val)
            {
                _type = EType::INTEGER;
                _double = (double)val;
                _integer = val;
                return (*this);
            }

            /**
             * Assigns this Json value to a boolean
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const bool val)
            {
                _type = EType::BOOLEAN;
                _bool = val;
                return (*this);
            }

            /**
             * Assigns this Json value to a string
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const String &val)
            {
                _type = EType::STRING;
                _string = val;
                return (*this);
            }

            /**
             * Assigns this Json value to a string
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const char *val)
            {
                _type = EType::STRING;
                _string = val;
                return (*this);
            }

            /**
             * Assigns this Json value to an object
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const Object &val)
            {
                _type = EType::OBJECT;
                _object = memory::MakeUnique<Object>(val);
                return (*this);
            }

            /**
             * Assigns this Json value to an array
             * @param val new value
             * @return Json&
             */
            inline Json &operator=(const Array &val)
            {
                _type = EType::ARRAY;
                _array = memory::MakeUnique<Array>(val);
                return (*this);
            }

            /**
             * Returns the type of this Json value
             * @return Json::Type object
             */
            inline _bpf_internal_json_type Type() const noexcept
            {
                return (_bpf_internal_json_type(_type));
            }

            /**
             * Compares a Json value with a number
             * @param other value to compare with
             * @return true if this is equal to other, false otherwise
             */
            inline bool operator==(double other) const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    return (false);
                return (_double == other);
            }

            /**
             * Compares a Json value with a number
             * @param other value to compare with
             * @return true if this is equal to other, false otherwise
             */
            inline bool operator==(int64 other) const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    return (false);
                return (_integer == other);
            }

            /**
             * Compares a Json value with a boolean
             * @param other value to compare with
             * @return true if this is equal to other, false otherwise
             */
            inline bool operator==(bool other) const
            {
                if (_type != EType::BOOLEAN)
                    return (false);
                return (_bool == other);
            }

            /**
             * Compares a Json value with a string
             * @param other value to compare with
             * @return true if this is equal to other, false otherwise
             */
            inline bool operator==(const String &other) const
            {
                if (_type != EType::STRING)
                    return (false);
                return (_string == other);
            }

            /**
             * Compares a Json value with a string
             * @param other value to compare with
             * @return true if this is equal to other, false otherwise
             */
            inline bool operator==(const char *other) const
            {
                if (_type == EType::NONE && other == nullptr)
                    return (true);
                if (_type != EType::STRING)
                    return (false);
                return (_string == String(other));
            }

            /**
             * Converts this value to a number
             * @throw JsonException if the value type is not compatible
             * @return double
             */
            inline operator double() const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    throw JsonException("Incompatible value type");
                return (_double);
            }

            /**
             * Converts this value to a number
             * @throw JsonException if the value type is not compatible
             * @return integer 64 bits
             */
            inline operator int64() const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    throw JsonException("Incompatible value type");
                return (_integer);
            }

            /**
             * Converts this value to a boolean
             * @throw JsonException if the value type is not compatible
             * @return bool
             */
            inline operator bool() const
            {
                if (_type != EType::BOOLEAN)
                    throw JsonException("Incompatible value type");
                return (_bool);
            }

            /**
             * Converts this value to a string
             * @throw JsonException if the value type is not compatible
             * @return immutable high-level string reference
             */
            inline operator const String &() const
            {
                if (_type != EType::STRING)
                    throw JsonException("Incompatible value type");
                return (_string);
            }

            /**
             * Converts this value to a Json Array
             * @throw JsonException if the value type is not compatible
             * @return immutable Array reference
             */
            inline operator const Array &() const
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            /**
             * Converts this value to a Json Object
             * @throw JsonException if the value type is not compatible
             * @return immutable Object reference
             */
            inline operator const Object &() const
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }

            /**
             * Converts this value to a Json Array
             * @throw JsonException if the value type is not compatible
             * @return mutable Array reference
             */
            inline operator Array &()
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            /**
             * Converts this value to a Json Object
             * @throw JsonException if the value type is not compatible
             * @return mutable Object reference
             */
            inline operator Object &()
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }

            /**
             * Converts this value to a number
             * @throw JsonException if the value type is not compatible
             * @return double
             */
            inline double ToDouble() const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    throw JsonException("Incompatible value type");
                return (_double);
            }

            /**
             * Converts this value to a number
             * @throw JsonException if the value type is not compatible
             * @return integer 64 bits
             */
            inline int64 ToInteger() const
            {
                if (_type != EType::DOUBLE && _type != EType::INTEGER)
                    throw JsonException("Incompatible value type");
                return (_integer);
            }

            /**
             * Converts this value to a boolean
             * @throw JsonException if the value type is not compatible
             * @return bool
             */
            inline bool ToBoolean() const
            {
                if (_type != EType::BOOLEAN)
                    throw JsonException("Incompatible value type");
                return (_bool);
            }

            /**
             * Converts this value to a string
             * @throw JsonException if the value type is not compatible
             * @return immutable high-level string reference
             */
            inline const String &ToString() const
            {
                if (_type != EType::STRING)
                    throw JsonException("Incompatible value type");
                return (_string);
            }

            /**
             * Converts this value to a Json Array
             * @throw JsonException if the value type is not compatible
             * @return immutable Array reference
             */
            inline const Array &ToArray() const
            {
                if (_type != EType::ARRAY)
                    throw JsonException("Incompatible value type");
                return (*_array);
            }

            /**
             * Converts this value to a Json Object
             * @throw JsonException if the value type is not compatible
             * @return immutable Object reference
             */
            inline const Object &ToObject() const
            {
                if (_type != EType::OBJECT)
                    throw JsonException("Incompatible value type");
                return (*_object);
            }
        };
    }
}
