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

#include <chrono>
#include "Framework/Profiler.hpp"

using namespace bpf;

Profiler::Profiler()
    : CurCreationID(0)
{
}

void Profiler::Push(const bpf::String &name)
{
    ProfilerSection section;

    section.Name = name;
    section.Time = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    section.Pos = _stack.Size();
    if (!_map.HasKey(name))
    {
        section.CreationID = CurCreationID++;
        _map[name] = section;
    }
    else
        section.CreationID = _map[name].CreationID;
    _stack.Push(section);
}

Array<ProfilerSection> Profiler::GenDisplayList()
{
    Array<ProfilerSection> sections = Array<ProfilerSection>(CurCreationID);
    for (auto &it : _map)
        sections[it.Value.CreationID] = it.Value;
    return (sections);
}

void Profiler::Pop()
{
    long long curt = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    ProfilerSection section = _stack.Pop();

    section.Time = curt - section.Time;
    _map[section.Name] = section;
}

Profiler &Profiler::Instance()
{
    static Profiler inst;

    return (inst);
}
