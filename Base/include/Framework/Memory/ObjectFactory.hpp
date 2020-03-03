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
#include "Framework/String.hpp"
#include "Framework/Memory/IObjectConstructor.hpp"
#include "Framework/Collection/Map.hpp"

namespace bpf
{
    namespace memory
    {
        /**
         * Factory, creates object instances from object constructor names
         * @tparam T the type of parent to generate
         * @tparam Args the arguments to the constructor
         */
        template <class /* ? extends */ T, typename ...Args>
        class BPF_API ObjectFactory
        {
        private:
            collection::Map<String, IObjectConstructor<T, Args...> *> Registry;

        public:
            template <class ObjConstructor>
            inline void AddClass()
            {
                IObjectConstructor<T, Args...> *raw = Memory::New<ObjConstructor>(); // TODO : Update when Map will accept UniquePtr as a value
                //WARNING : MemLeak to fix here
                Registry[raw->GetName()] = raw;
            }

            inline UniquePtr<T> MakeUnique(const String &name, Args &&... args)
            {
                if (!Registry.HasKey(name))
                    return (Null);
                return (Registry[name]->MakeUnique(std::forward<Args>(args)...));
            }

            inline SharedPtr<T> MakeShared(const String &name, Args &&... args)
            {
                if (!Registry.HasKey(name))
                    return (Null);
                return (Registry[name]->MakeShared(std::forward<Args>(args)...));
            }
        };
    }
}
