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
#include "Framework/System/Module.hpp"
#include "Framework/Collection/HashMap.hpp"
#include "Framework/Memory/Utility.hpp"

namespace bpf
{
    namespace system
    {
        template <typename BaseClass>
        class BP_TPL_API ModuleManager
        {
        private:
            typedef memory::UniquePtr<BaseClass> (*ModuleLinkFunc)();
            typedef fint(*ModuleDescribeFunc)();

            struct Entry
            {
                memory::UniquePtr<BaseClass> Interface;
                Module Handle;
            };

            String _modulePath;
            collection::HashMap<String, Entry> _map;

        public:
            inline ModuleManager(const String &modulePath)
                : _modulePath(modulePath)
            {
            }

            ModuleManager(const ModuleManager &other) = delete;
            ModuleManager &operator=(const ModuleManager &other) = delete;

            /**
             * Loads the given module name
             * @throws ModuleException
             */
            void LoadModule(const String &virtualName, const String &fileName);

            /**
             * Unloads the given module name
             */
            void UnloadModule(const String &virtualName);

            inline bool HasModule(const String &virtualName) const noexcept
            {
                return (_map.HasKey(virtualName));
            }

            inline const memory::UniquePtr<BaseClass> &GetModule(const String &virtualName) const noexcept
            {
                if (!HasModule(virtualName))
                    return (memory::UniquePtr<BaseClass>::NullPtr);
                else
                    return (_map[virtualName].Interface);
            }
        };
    }
};

#include "Framework/System/ModuleManager.impl.hpp"