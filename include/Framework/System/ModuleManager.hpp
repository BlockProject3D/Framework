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

