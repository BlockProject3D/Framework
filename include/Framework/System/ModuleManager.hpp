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

#ifndef MODULEMANAGER_H_
# define MODULEMANAGER_H_

#include <map>
#include "Framework/System/Module.hpp"
#include "Framework/System/IModuleInterface.hpp"

# ifdef WINDOWS
#  define IMPLEMENT_MODULE(name, clname) \
extern "C" \
{ \
    __declspec(dllexport) IModuleInterface *name##_Link() \
    { \
        return (new clname()); \
    } \
} \

# else
#  define IMPLEMENT_MODULE(name, clname) \
extern "C" \
{ \
    Framework::IModuleInterface *name##_Link() \
    { \
        return (new clname()); \
    } \
    int	name##_Describe() \
    { \
        return (VERSION_ENGINE_INT); \
    } \
} \

# endif

namespace bpf
{
    typedef IModuleInterface*(*ModuleLinkFunc)();
    typedef int(*ModuleDescribeFunc)();

    class BPF_API ModuleEntry
    {
    public:
        IModuleInterface *Interface;
        bpf::String Name;
        Module Module;
        inline ModuleEntry(const bpf::String &path, const bpf::String &name)
            : Interface(Null), Name(name), Module(path)
        {
        }
    };
    
    class BPF_API ModuleManager
    {
    private:
        bpf::List<ModuleEntry *> ModuleList;
        bpf::Map<const char *, ModuleEntry *> ModuleMap;

        void UnloadModule(ModuleEntry *entry);
    public:
        inline ModuleManager() {}
        ~ModuleManager();
        
        /**
         * Loads the given module name
         * @throws ModuleException
         */
        void LoadModule(const char *name);

        /**
         * Unloads the given module name
         */
        void UnloadModule(const char *name);

        inline bool ModuleLoaded(const char *name)
        {
            return (ModuleMap.HasKey(name));
        }

        template <typename T>
        inline T *GetModule(const char *name)
        {
            if (ModuleMap.HasKey(name))
                return (dynamic_cast<T *>(ModuleMap[name]->Interface));
            else
                return (Null);
        }
    };
};

#endif /* !MODULEMANAGER_H_ */

