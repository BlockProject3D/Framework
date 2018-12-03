#ifndef MODULEMANAGER_H_
# define MODULEMANAGER_H_

# include <map>
//# include "engine/version.h"

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

namespace Framework
{
    class IModuleInterface
    {
    public:
        virtual ~IModuleInterface() {}
        virtual void OnLoadModule() = 0;
        virtual void OnUnloadModule() = 0;
    };

    typedef IModuleInterface*(*ModuleLinkFunc)();
    typedef int(*ModuleDescribeFunc)();

    class ENGINE_API FModuleEntry
    {
    public:
        IModuleInterface *Interface;
        FString Name;
        FModule Module;
        inline FModuleEntry(const FString &path, const FString &name)
            : Interface(Null), Name(name), Module(path)
        {
        }
    };
    
    class ENGINE_API FModuleManager
    {
    private:
        static FLogger *Log;
        static FList<FModuleEntry *> ModuleList;
        static FMap<const char *, FModuleEntry *> ModuleMap;

        static void UnloadModule(FModuleEntry *entry);
    public:
        static void Initialize();
        static void Shutdown();
        static bool LoadModule(const char *name);
        static void UnloadModule(const char *name);

        inline static bool ModuleLoaded(const char *name)
        {
            return (ModuleMap.HasKey(name));
        }

        template<typename T>
        static inline T	*GetModule(const char *name)
        {
            if (ModuleMap.HasKey(name))
                return (dynamic_cast<T *>(ModuleMap[name]->Interface));
            else
                return (Null);
        }
    };
};

#endif /* !MODULEMANAGER_H_ */

