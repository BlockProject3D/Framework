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
#include "Framework/IO/File.hpp"
#include "Framework/System/Module.hpp"
#include "Framework/Collection/HashMap.hpp"
#include "Framework/Memory/Utility.hpp"
#include "Framework/Name.hpp"

namespace bpf
{
    namespace system
    {
        template <typename BaseClass>
        class BP_TPL_API ModuleManager
        {
        private:
            typedef BaseClass *(*ModuleLinkFunc)();
            typedef fint(*ModuleDescribeFunc)();

            struct Entry
            {
                memory::UniquePtr<BaseClass> Interface;
                Module Handle;
            };

            io::File _modulePath;
            collection::HashMap<Name, Entry> _map;

        public:
            explicit inline ModuleManager(const io::File &modulePath)
                : _modulePath(modulePath)
            {
            }

            ~ModuleManager();

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
            void UnloadModule(const Name &virtualName);

            inline bool HasModule(const Name &virtualName) const noexcept
            {
                return (_map.HasKey(virtualName));
            }

            inline const memory::UniquePtr<BaseClass> &GetModule(const Name &virtualName) const noexcept
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