#include <stdlib.h>
#ifdef WINDOWS
    #include <Windows.h>
#elif LINUX
    #include <sys/utsname.h>
    #include <sys/sysinfo.h>
#elif MAC
    #include <CoreServices/CoreServices.h>
#endif
#include <intrin.h>

#undef ERROR

#include "Framework/System/Platform.hpp"
#include "Framework/System/ModuleManager.hpp"
#include "Framework/IO/FileSystem.hpp"
#include "Framework/System/System.hpp"
#include "Framework/System/TypeExpander.hpp"

namespace bpf
{
#ifdef WINDOWS
    typedef LONG NTSTATUS, *PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

    RTL_OSVERSIONINFOW GetRealOSVersion() {
        HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
        if (hMod) {
            RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
            if (fxPtr != nullptr) {
                RTL_OSVERSIONINFOW rovi = { 0 };
                rovi.dwOSVersionInfoSize = sizeof(rovi);
                if (STATUS_SUCCESS == fxPtr(&rovi)) {
                    return rovi;
                }
            }
        }
        RTL_OSVERSIONINFOW rovi = { 0 };
        return rovi;
    }
#endif

    Env Platform::InitEnvInfo()
    {
        Env ev;

        ev.ShortName = "BPF";
        ev.Name = "BlockProject Framework";
        ev.Version = "XR";
        ev.VersionInt = 0x1052;
#ifdef BUILD_DEBUG
        ev.VersionInt |= 0x00001;
#endif
        return (ev);
    }

    OS Platform::InitOSInfo()
    {
        OS os;

#ifdef WINDOWS
        os.ModuleExt = "dll";
        os.Name = "Windows";
        os.NewLine = "\r\n";
        os.PathSep = "\\";
        RTL_OSVERSIONINFOW ver = GetRealOSVersion();
        os.Version = String::ValueOf(ver.dwMajorVersion)
            + "." + String::ValueOf(ver.dwMinorVersion);
#elif LINUX
        os.ModuleExt = "so";
        os.Name = "Linux";
        os.NewLine = "\n";
        os.PathSep = "/";
        struct utsname st;
        if (uname(&st) != -1)
            os.Version = st.version;
#elif MAC
        os.ModuleExt = "dylib";
        os.Name = "Mac";
        os.NewLine = "\n";
        os.PathSep = "/";
        SInt32 majorVersion, minorVersion, bugFixVersion;
        Gestalt(gestaltSystemVersionMajor, &majorVersion);
        Gestalt(gestaltSystemVersionMinor, &minorVersion);
        Gestalt(gestaltSystemVersionBugFix, &bugFixVersion);
        os.Version = String::ValueOf(majorVersion) + "." + String::ValueOf(minorVersion)
            + "." + String::ValueOf(bugFixVersion);
#endif
        return (os);
    }

    Env &Platform::GetEnvInfo()
    {
        static Env ev = Platform::InitEnvInfo();

        return (ev);
    }

    OS &Platform::GetOSInfo()
    {
        static OS os = Platform::InitOSInfo();

        return (os);
    }

    CPU Platform::GetCPUInfo()
    {
        CPU cpi;

#ifdef WINDOWS
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        cpi.NumCores = sysInfo.dwNumberOfProcessors;
        LARGE_INTEGER lg;
        QueryPerformanceFrequency(&lg);
        cpi.Freq = lg.HighPart;
#elif LINUX
        cpi.NumCores = get_nprocs();
        cpi.Freq = 0;
#else
        cpi.NumCores = 0;
        cpi.Freq = 0;
#endif
        int cpuInfo[4] = { -1 };
        char CPUBrandString[0x40];
        memset(CPUBrandString, 0, sizeof(CPUBrandString));
        __cpuid(cpuInfo, 0x80000002);
        memcpy(CPUBrandString, cpuInfo, sizeof(cpuInfo));
        __cpuid(cpuInfo, 0x80000003);
        memcpy(CPUBrandString + 16, cpuInfo, sizeof(cpuInfo));
        __cpuid(cpuInfo, 0x80000004);
        memcpy(CPUBrandString + 32, cpuInfo, sizeof(cpuInfo));
        cpi.Name = String(CPUBrandString);
    }

    RAM Platform::GetRAMInfo()
    {
        RAM rami;

#ifdef WINDOWS
        MEMORYSTATUSEX meminfo;
        GlobalMemoryStatusEx(&meminfo);
        rami.MaxPhysical = static_cast<uint64>(meminfo.ullTotalPhys);
        rami.MaxVirtual = static_cast<uint64>(meminfo.ullTotalVirtual);
#else
        rami.MaxPhysical = 0;
        rami.MaxVirtual = 0;
#endif
    }
}

using namespace Framework;

bool FPlatform::Exit = false;
IFileSystem *FPlatform::FileSys = Null;
ISystemManager *FPlatform::BaseSys = Null;
bpf::Map<const char *, bpf::String> FPlatform::PropMap;

void FPlatform::RequestExit(bool force)
{
    if (force)
        exit(1);
    else
        Exit = true;
}

bool FPlatform::IsExiting()
{
    return (Exit);
}

IFileSystem *FPlatform::GetFileSystem()
{
    if (FileSys == NULL)
        FileSys = FModuleManager::GetModule<IFileSystem>("FileSystem");
    return (FileSys);
}

EPlatformEndianess FPlatform::GetPlatformEndianess()
{
    bpf::TypeExpander<uint32> data = bpf::TypeExpander<uint32>(0x1020304);

    if (data.Bytes[0] == 1)
        return (PLATFORM_BIGENDIAN);
    return (PLATFORM_LITTLEENDIAN);
}

void FPlatform::ReverseBuffer(uint8 *buf, const uint32 size)
{
    uint32 i = 0;
    uint32 j = size;
    uint8 temp;

    while (i < size / 2)
    {
        temp = buf[i];
        buf[i++] = buf[j];
        buf[j] = temp;
        j--;
    }
}

void FPlatform::ReverseBuffer(uint8 *buf, const uint32 groupsize, const uint32 size)
{
    uint32 i = 0;
    uint32 j = size;
    uint8 temp;

    while (i < size / 2)
    {
        for (uint32 k = 0 ; k < groupsize ; ++k)
        {
            if (i + k >= size || j + k >= size)
                continue;
            temp = buf[i + k];
            buf[i + k] = buf[j + k];
            buf[j + k] = temp;
        }
        i += groupsize;
        j -= groupsize;
    }
}

ISystemManager *FPlatform::GetBaseSystem()
{
    if (BaseSys == NULL)
        BaseSys = FModuleManager::GetModule<ISystemManager>("System");
    return (BaseSys);
}

void FPlatform::Initialize()
{
#if LINUX
    SetProperty("OS_MODULE_EXT", ".so");
    SetProperty("OS_NAME", "Linux");
    SetProperty("RENDER_DEFAULT", "GL33");
#elif MAC
    SetProperty("OS_MODULE_EXT", ".dylib");
    SetProperty("OS_NAME", "Mac OSX");
    SetProperty("RENDER_DEFAULT", "GL33");
#elif WINDOWS
    SetProperty("OS_MODULE_EXT", ".dll");
    SetProperty("OS_NAME", "Windows");
    SetProperty("RENDER_DEFAULT", "GL33");
#endif
    SetProperty("ENGINE_NAME", "BPEngine");
    SetProperty("ENGINE_LONG_NAME", "BlockProjectEngine");
    if (sizeof(int *) > 4)
        SetProperty("ARCHITECTURE", "x64");
    else
        SetProperty("ARCHITECTURE", "x86");
    FModuleManager::Initialize();
}

void FPlatform::Shutdown()
{
    FModuleManager::Shutdown();
}

const bpf::String &FPlatform::GetProperty(const char *vname)
{
    if (PropMap.HasKey(vname))
        return (PropMap[vname]);
    return (bpf::String::Empty);
}

void FPlatform::SetProperty(const char *vname, const bpf::String &prop)
{
    PropMap[vname] = prop;
}
