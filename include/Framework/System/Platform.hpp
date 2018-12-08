#pragma once

#include "Framework/Framework.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    class String;

    struct OS
    {
        String Name;
        String Version;
        String ModuleExt;
        String PathSep;
        String NewLine;
    };

    struct CPU
    {
        String Name;
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
        String ShortName;
        String Name;
        String Version;
        int VersionInt;
    };

    class BPF_API Platform
    {
    private:
        static Env InitEnvInfo();
        static OS InitOSInfo();

    public:
        static Env &GetEnvInfo();
        static OS &GetOSInfo();
        static CPU GetCPUInfo();
        static RAM GetRAMInfo();
    };
}

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
