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

#include "Framework/RuntimeException.hpp"
#include "Framework/Memory/MemUtils.hpp"
#include <iostream>

using namespace bpf;

RuntimeException::RuntimeException(const String &type, const String &message)
    : _type(memory::MemUtils::New<String>(type + "Exception"))
    , _message(memory::MemUtils::New<String>(message))
    , _refs(reinterpret_cast<int *>(memory::Memory::Malloc(sizeof(int))))
{
    *_refs = 1;
}

RuntimeException::~RuntimeException()
{
    *_refs -= 1;
    if (*_refs <= 0)
    {
        memory::MemUtils::Delete(_type);
        memory::MemUtils::Delete(_message);
        memory::Memory::Free(_refs);
    }
}

RuntimeException::RuntimeException(const RuntimeException &other) noexcept
    : _type(other._type)
    , _message(other._message)
    , _refs(other._refs)
{
    *_refs += 1;
}

RuntimeException &RuntimeException::operator=(const RuntimeException &other) noexcept
{
    if (this == &other)
        return (*this);
    *other._refs += 1;
    *_refs -= 1;
    if (*_refs <= 0)
    {
        memory::MemUtils::Delete(_type);
        memory::MemUtils::Delete(_message);
        memory::Memory::Free(_refs);
    }
    _type = other._type;
    _message = other._message;
    _refs = other._refs;
    return (*this);
}

void RuntimeException::Print() const noexcept
{
    std::cerr << Type() << " thrown: " << std::endl;
    std::cerr << *Message() << std::endl;
}
