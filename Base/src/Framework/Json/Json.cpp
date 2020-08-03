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

#include "Framework/Json/Json.hpp"

using namespace bpf::memory;
using namespace bpf::json;
using namespace bpf;

Json::Array::Array(const std::initializer_list<Json> &vals)
{
    for (auto &it : vals)
        Items.Add(it);
}

Json::Object::Object(const std::initializer_list<std::pair<String, Json>> &lst)
{
    for (auto &it : lst)
        Properties[it.first] = it.second;
}

Json &Json::operator=(const Json &other)
{
    if (this == &other)
        return (*this);
    _type = other._type;
    _double = other._double;
    _integer = other._integer;
    _bool = other._bool;
    _string = other._string;
    _array = other._array != nullptr ? MakeUnique<Array>(*other._array) : nullptr;
    _object = other._object != nullptr ? MakeUnique<Object>(*other._object) : nullptr;
    return (*this);
}

Json &Json::operator=(Json &&other) noexcept
{
    _type = other._type;
    _double = other._double;
    _integer = other._integer;
    _bool = other._bool;
    _string = std::move(other._string);
    _array = std::move(other._array);
    _object = std::move(other._object);
    return (*this);
}
