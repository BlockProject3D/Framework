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
#include "Framework/Types.hpp"
#include "Framework/String.hpp"
#include "Framework/Collection/Stack.hpp"
#include "Framework/Collection/Array.hpp"
#include "Framework/Collection/HashMap.hpp"

/**
 * OLD FRAMEWORK START / UNSUPPORTED
 * Old Framework utilities are unsupported and may be removed in a future release
 */

# ifdef BUILD_DEBUG
#  define PROFILER_PUSH_SECTION(name) Framework::FProfiler::PushSection(name)
#  define PROFILER_POP_SECTION() Framework::FProfiler::PopSection()
# else
#  define PROFILER_PUSH_SECTION(name)
#  define PROFILER_POP_SECTION()
# endif

namespace bpf
{
    struct BPF_API ProfilerSection
    {
        String Name;
        long long Time; //In micro seconds
        fint Pos;
        uint32 CreationID;
    };

    class BPF_API Profiler
    {
    private:
        uint32 CurCreationID;
        collection::HashMap<String, ProfilerSection> _map;
        collection::Stack<ProfilerSection> _stack = collection::Stack<ProfilerSection>(32);

        void Push(const String &name);
        void Pop();
        Profiler();

    public:
        static Profiler &Instance();

        BP_DEPRECATED("Old Framework utilities are unsupported and may be removed in a future release")
        collection::Array<ProfilerSection> GenDisplayList();

        BP_DEPRECATED("Old Framework utilities are unsupported and may be removed in a future release")
        inline static void PushSection(const String &name)
        {
            Instance().Push(name);
        }

        BP_DEPRECATED("Old Framework utilities are unsupported and may be removed in a future release")
        inline static void PopSection()
        {
            Instance().Pop();
        }
    };
}

/**
 * OLD FRAMEWORK END
 */
