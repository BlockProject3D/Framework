#ifndef PLATFORM_H_
# define PLATFORM_H_

# include <initializer_list>
#include "Framework/Framework.hpp"

namespace bpf
{
    class String;

    /*struct OS
    {
        String Name;
        String Version;
        String ModuleExt;
        String PathSep;
        String NewLine;
    };

    struct CPU
    {
        int NumCores;
        int Freq;
    };

    struct RAM
    {
        uint64 MaxPhysical;
        uint64 MaxVirtual;
    };

    struct Env
    {
        String Name;
        String Version;
    };*/

} //TODO : Remove

namespace Framework
{
    class IFileSystem;
    class ISystemNetwork;
    class ISystemManager;
    class FString;

    enum EPlatformEndianess
    {
        PLATFORM_BIGENDIAN,
        PLATFORM_LITTLEENDIAN
    };

    template <typename T>
    union FTypeExpander
    {
        T		Data;
        uint8	Bytes[sizeof(T)];
        inline FTypeExpander(T d)
            : Data(d)
	{
	}

        inline FTypeExpander(std::initializer_list<uint8> d)
        {
            int i = 0;

            for (auto it = d.begin() ; it != d.end() ; ++it)
                Bytes[i++] = *it;
        }

        inline FTypeExpander()
            : Data(0)
	{
	}
    };

    class ENGINE_API FPlatform
    {
    private:
        static bool Exit;
        static IFileSystem *FileSys;
        static ISystemManager *BaseSys;
        static bpf::Map<const char *, bpf::String> PropMap;
    public:
        static void RequestExit(bool force=false);
        static bool IsExiting();
        static IFileSystem *GetFileSystem();
        static ISystemManager *GetBaseSystem();
        static EPlatformEndianess GetPlatformEndianess();
        static void ReverseBuffer(uint8 *buf, const uint32 size);

        /**
         * Reverse buffer buf groupsize per groupsize instead of byte per byte
         */
        static void ReverseBuffer(uint8 *buf, const uint32 groupsize, const uint32 size);
        static void Initialize();
        static void Shutdown();
        static const bpf::String &GetProperty(const char *vname);
        static void SetProperty(const char *vname, const bpf::String &prop);
    };
};

#endif /* !PLATFORM_H_ */
