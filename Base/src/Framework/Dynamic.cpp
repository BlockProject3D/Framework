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

#include "Framework/Dynamic.hpp"

using namespace bpf;

Dynamic::~Dynamic()
{
    memory::MemUtils::Delete(_storage);
}

Dynamic &Dynamic::operator=(const Dynamic &other)
{
    if (this == &other)
        return (*this);
    memory::MemUtils::Delete(_storage);
    _storage = other._storage == nullptr ? nullptr : other._storage->Clone();
    return (*this);
}

Dynamic &Dynamic::operator=(Dynamic &&other) noexcept
{
    memory::MemUtils::Delete(_storage);
    _storage = other._storage;
    other._storage = nullptr;
    return (*this);
}

bool Dynamic::operator==(const Dynamic &other) const
{
    if (_storage == nullptr || other._storage == nullptr)
        return (_storage == other._storage);
    return (_storage->Equals(other._storage));
}

bool Dynamic::operator!=(const Dynamic &other) const
{
    if (_storage == nullptr || other._storage == nullptr)
        return (_storage != other._storage);
    return (!_storage->Equals(other._storage));
}
