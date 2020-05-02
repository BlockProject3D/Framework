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

#include <stdlib.h>
#ifdef WINDOWS
    #include <Windows.h>
    #include <intrin.h>
    #undef ERROR
#elif defined(LINUX) || defined(ANDROID)
    #include <sys/sysinfo.h>
    #include <sys/utsname.h>
    #include <time.h>
#else
    #include <sys/sysctl.h>
    #include <sys/types.h>
#endif
#ifndef WINDOWS
    #include <unistd.h>
#endif

#ifndef WINDOWS //We assume compiler supports GCC style asm

    #define INSTRUCTION_CPUID(val)   \
        asm("movl $" #val ", %eax"); \
        asm("cpuid")

    #define READ_REGISTER(name, val) \
        asm("movl %%" #name ", %0"   \
            : "=r"(val)              \
            :)

#endif

#include "Framework/System/ModuleInterface.hpp"
#include "Framework/System/Platform.hpp"
#include "Framework/System/TypeExpander.hpp"

using namespace bpf::system;
using namespace bpf;

#ifdef WINDOWS
typedef LONG NTSTATUS, *PNTSTATUS;
    #define STATUS_SUCCESS (0x00000000)

typedef NTSTATUS(WINAPI *RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

RTL_OSVERSIONINFOW GetRealOSVersion()
{
    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod)
    {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != nullptr)
        {
            RTL_OSVERSIONINFOW rovi = {0};
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (STATUS_SUCCESS == fxPtr(&rovi))
            {
                return rovi;
            }
        }
    }
    RTL_OSVERSIONINFOW rovi = {0};
    return rovi;
}
#endif

String Platform::CPUIDIntToStr(fint data)
{
    String res = "";
    TypeExpander<int> exp(data);
    res += static_cast<char>(exp.Bytes[0]);
    res += static_cast<char>(exp.Bytes[1]);
    res += static_cast<char>(exp.Bytes[2]);
    res += static_cast<char>(exp.Bytes[3]);
    return (res);
}

String Platform::IdentifyCPUBranding()
{
#ifdef WINDOWS
    fint cpuInfo[4] = {-1};
    char CPUBrandString[0x40];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));
    __cpuid(cpuInfo, 0x80000002);
    memcpy(CPUBrandString, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000003);
    memcpy(CPUBrandString + 16, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000004);
    memcpy(CPUBrandString + 32, cpuInfo, sizeof(cpuInfo));
    return (String(CPUBrandString));
#else
    #if defined(__arm__) || defined(__aarch64__)
    //For getting a name here go ask ARM architecture team to provide the missing cpuid instruction or an instruction that can obtain brand name
    return ("Generic ARM Processor");
    #else
        #ifdef BUILD_DEBUG
    String res = "";
    fint reg_eax = 0;
    fint reg_ebx = 0;
    fint reg_ecx = 0;
    fint reg_edx = 0;

    INSTRUCTION_CPUID(0x80000002);
    READ_REGISTER(eax, reg_eax);
    READ_REGISTER(ebx, reg_ebx);
    READ_REGISTER(ecx, reg_ecx);
    READ_REGISTER(edx, reg_edx);
    res += CPUIDIntToStr(reg_eax);
    res += CPUIDIntToStr(reg_ebx);
    res += CPUIDIntToStr(reg_ecx);
    res += CPUIDIntToStr(reg_edx);
    INSTRUCTION_CPUID(0x80000003);
    READ_REGISTER(eax, reg_eax);
    READ_REGISTER(ebx, reg_ebx);
    READ_REGISTER(ecx, reg_ecx);
    READ_REGISTER(edx, reg_edx);
    res += CPUIDIntToStr(reg_eax);
    res += CPUIDIntToStr(reg_ebx);
    res += CPUIDIntToStr(reg_ecx);
    res += CPUIDIntToStr(reg_edx);
    INSTRUCTION_CPUID(0x80000004);
    READ_REGISTER(eax, reg_eax);
    READ_REGISTER(ebx, reg_ebx);
    READ_REGISTER(ecx, reg_ecx);
    READ_REGISTER(edx, reg_edx);
    res += CPUIDIntToStr(reg_eax);
    res += CPUIDIntToStr(reg_ebx);
    res += CPUIDIntToStr(reg_ecx);
    res += CPUIDIntToStr(reg_edx);
    return (res);
        #else
    return ("Generic x86_64 CPU"); //In optimized build attempting to call cpuid crashes the application under unix
        #endif
    #endif
#endif
}

Env Platform::InitEnvInfo()
{
    Env ev;

    ev.ShortName = "BPF";
    ev.Name = "BlockProject Framework";
    ev.Version = "2.6";
    ev.VersionInt = BP_MODULE_VERSION_INT;
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
    os.Version = String::ValueOf(static_cast<int>(ver.dwMajorVersion)) + "." + String::ValueOf(static_cast<int>(ver.dwMinorVersion));
#elif defined(LINUX) || defined(ANDROID)
    os.ModuleExt = "so";
    os.Name = "Linux";
    os.NewLine = "\n";
    os.PathSep = "/";
    struct utsname st;
    if (uname(&st) != -1)
        os.Version = st.version;
#else
    os.ModuleExt = "dylib";
    os.Name = "Mac";
    os.NewLine = "\n";
    os.PathSep = "/";
    os.Version = "Impossible - Ask Apple why they deprecate C++ APIs !";
#endif
    return (os);
}

const Env &Platform::GetEnvInfo()
{
    static Env ev = Platform::InitEnvInfo();

    return (ev);
}

const OS &Platform::GetOSInfo()
{
    static OS os = Platform::InitOSInfo();

    return (os);
}

const CPU &Platform::GetCPUInfo()
{
    static CPU cpi = {IdentifyCPUBranding(), 0, 0};

#ifdef WINDOWS
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpi.NumCores = sysInfo.dwNumberOfProcessors;
    cpi.Freq = 1; //Cannot reliably find CPU frequency
#elif LINUX
    cpi.NumCores = get_nprocs();
    cpi.Freq = 1; //Cannot reliably find CPU frequency
#else
    fint ncores = 1;
    size_t sz = sizeof(fint);
    if (sysctlbyname("hw.activecpu", &ncores, &sz, Null, 0))
        sysctlbyname("hw.ncpu", &ncores, &sz, Null, 0);
    cpi.NumCores = ncores;
    cpi.Freq = 1; //Cannot reliably find CPU frequency
#endif
    return (cpi);
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
    auto pages = sysconf(_SC_PHYS_PAGES);
    auto page_size = sysconf(_SC_PAGE_SIZE);
    rami.MaxPhysical = static_cast<uint64>(pages * page_size);
    rami.MaxVirtual = static_cast<uint64>(pages * page_size);
#endif
    return (rami);
}

EPlatformEndianess Platform::GetEndianess()
{
    TypeExpander<uint32> data = TypeExpander<uint32>(0x1020304);

    if (data.Bytes[0] == 1)
        return (PLATFORM_BIGENDIAN);
    return (PLATFORM_LITTLEENDIAN);
}

void Platform::ReverseBuffer(void *buf, const fsize size)
{
    uint8 *out = reinterpret_cast<uint8 *>(buf);
    fsize i = 0;
    fsize j = size - 1;
    uint8 temp;

    while (i < size / 2)
    {
        temp = out[i];
        out[i++] = out[j];
        out[j] = temp;
        j--;
    }
}

void Platform::ReverseBuffer(void *buf, const fsize size, const fsize groupsize)
{
    uint8 *out = reinterpret_cast<uint8 *>(buf);
    fsize i = 0;
    fsize j = size - groupsize;
    uint8 temp;

    while (i < size / 2)
    {
        for (fsize k = 0; k < groupsize; ++k)
        {
            if (i + k >= size || j + k >= size)
                continue;
            temp = out[i + k];
            out[i + k] = out[j + k];
            out[j + k] = temp;
        }
        i += groupsize;
        j -= groupsize;
    }
}
