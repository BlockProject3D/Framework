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

constexpr int PIPE_WRITE = 1;
constexpr int PIPE_READ = 0;

#include "Framework/System/Process.hpp"
#include "../IO/OSPrivate.hpp"
#include "Framework/IO/IOException.hpp"
#include "Framework/Scalar.hpp"
#include "Framework/System/OSException.hpp"
#ifndef WINDOWS
    #include "Framework/System/Thread.hpp"
#endif
#ifdef WINDOWS
    #include "Framework/Collection/ArrayList.hpp"
    #include <Windows.h>
    #include <cstring>
    #include <set>
#elif LINUX
    #include <fcntl.h>
    #include <cstring>
    #include <unistd.h>
    #include <wait.h>
#else
    #include <fcntl.h>
    #include <signal.h>
    #include <string.h>
    #include <sys/wait.h>
    #include <unistd.h>
#endif

#ifdef COVERAGE
extern "C" void __gcov_flush(void);
#endif

using namespace bpf;
using namespace bpf::io;
using namespace bpf::system;
using namespace bpf::collection;

Process::Builder::Builder()
    : _redirectStdIn(false)
    , _redirectStdOut(false)
    , _redirectStdErr(false)
    , _workDir(File("."))
{
}

Process::Builder::Builder(const Application &app)
    : _redirectStdIn(false)
    , _redirectStdOut(false)
    , _redirectStdErr(false)
    , _workDir(app.GetWorkingDirectory())
    , _envp(app.Environment)
{
}

Process::Builder &Process::Builder::SetApplication(const String &name)
{
    auto str = name; //NOLINT (Copy is required thanks WinAPI)
#ifdef WINDOWS
    if (str.Contains('/') || str.Contains('\\'))
#else
    if (str.Contains('/'))
#endif
    {
        auto f = File(str);
#ifdef WINDOWS
        if (!f.Exists())
            str += ".exe";
        f = File(str);
#endif
        if (f.Exists() && !f.IsDirectory())
        {
            _appExe = f.PlatformPath();
            _appName = f.Name();
        }
        else
            throw IOException(String("File '") + name + "' does not exist");
    }
    else
    {
#ifdef WINDOWS
        // Automatically add .exe file extension to improve cross platform compatibility
        if (name.LastIndexOf('.') == -1)
            str += ".exe";
#endif
        auto path = String(PROCESS_DEFAULT_PATH);
        if (_envp.HasKey("PATH"))
            path = _envp["PATH"];
#ifdef WINDOWS
        WCHAR buf[MAX_PATH];
        String sysDir = "C:/Windows";
        if (GetSystemDirectoryW(buf, MAX_PATH) != 0)
            sysDir = String::FromUTF16(reinterpret_cast<const fchar16 *>(buf));
        path = path.Replace("%SystemRoot%", sysDir);
        auto paths = path.Explode(';');
#else
        auto paths = path.Explode(':');
#endif
        for (auto &p : paths)
        {
            auto f = File(p) + File(str);
            if (f.Exists() && !f.IsDirectory())
            {
                _appExe = f.PlatformPath();
                _appName = f.Name();
                break;
            }
        }
        if (_appExe.IsEmpty())
            throw IOException(String("Could not find file '") + name + "' in PATH");
    }
    return (*this);
}

Process::Builder &Process::Builder::SetWorkingDirectory(const io::File &dir)
{
    if (!dir.Exists())
        throw IOException("Directory not found");
    if (!dir.IsDirectory())
        throw IOException("Not a directory");
    _workDir = dir;
    return (*this);
}

#ifdef WINDOWS

void Process::Builder::CleanupHandles(void *fdStdOut[2], void *fdStdErr[2], void *fdStdIn[2])
{
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdOut[i] != nullptr)
            CloseHandle(fdStdOut[i]);
    }
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdErr[i] != nullptr)
            CloseHandle(fdStdErr[i]);
    }
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdIn[i] != nullptr)
            CloseHandle(fdStdIn[i]);
    }
}

#else

void Process::Builder::CleanupHandles(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2])
{
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdOut[i] != -1)
            close(fdStdOut[i]);
    }
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdErr[i] != -1)
            close(fdStdErr[i]);
    }
    for (int i = 0; i != 2; ++i)
    {
        if (fdStdIn[i] != -1)
            close(fdStdIn[i]);
    }
    for (int i = 0; i != 2; ++i)
    {
        if (commonfd[i] != -1)
            close(commonfd[i]);
    }
}

void Process::Builder::ProcessWorker(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2])
{
    char **argv = reinterpret_cast<char **>(malloc(sizeof(char **) * (_argv.Size() + 1)));
    char **envp = reinterpret_cast<char **>(malloc(sizeof(char **) * (_envp.Size() + 1)));
    fsize i = 0;

    if (argv == nullptr || envp == nullptr)
        goto mallocerr;
    for (int fd = 0; fd != 256; ++fd)
    {
        if (fd != 0 && fd != 1 && fd != 2 && fd != fdStdOut[PIPE_WRITE] && fd != fdStdErr[PIPE_WRITE] &&
            fd != fdStdIn[PIPE_READ] && fd != commonfd[PIPE_WRITE])
            close(fd);
    }
    if (fcntl(commonfd[PIPE_WRITE], F_SETFD, FD_CLOEXEC) != 0)
    {
        BP_IGNORE(write(commonfd[PIPE_WRITE], "fcntl failure", 14))
        close(commonfd[PIPE_WRITE]);
        exit(1);
    }
    if (_redirectStdOut && dup2(fdStdOut[PIPE_WRITE], 1) == -1)
        goto redirecterr;
    if (_redirectStdErr && dup2(fdStdErr[PIPE_WRITE], 2) == -1)
        goto redirecterr;
    if (_redirectStdIn && dup2(fdStdIn[PIPE_READ], 0) == -1)
        goto redirecterr;
    BP_IGNORE(chdir(*_workDir.PlatformPath()))
    for (auto &a : _argv)
    {
        argv[i] = reinterpret_cast<char *>(malloc(a.Size() + 1));
        if (argv[i] == nullptr)
            goto mallocerr;
        memcpy(argv[i], *a, a.Size() + 1); // Copy with additional '\0'
        ++i;
    }
    argv[i] = nullptr;
    i = 0;
    for (auto &kv : _envp)
    {
        envp[i] = reinterpret_cast<char *>(malloc(kv.Key.Size() + kv.Value.Size() + 2));
        if (envp[i] == nullptr)
            goto mallocerr;
        memcpy(envp[i], *kv.Key, kv.Key.Size());
        envp[i][kv.Key.Size()] = '=';
        memcpy(envp[i] + kv.Key.Size() + 1, *kv.Value, kv.Value.Size() + 1);
        ++i;
    }
    envp[i] = nullptr;
    #ifdef COVERAGE
    __gcov_flush();
    #endif
    if (execve(*_appExe, argv, envp) == -1)
    {
        BP_IGNORE(write(commonfd[PIPE_WRITE], "execve failure", 15))
        close(commonfd[PIPE_WRITE]);
    #ifdef COVERAGE
        __gcov_flush();
    #endif
        exit(1);
    }
redirecterr:
    BP_IGNORE(write(commonfd[PIPE_WRITE], "Could not create one or more redirection(s)", 44))
    close(commonfd[PIPE_WRITE]);
    #ifdef COVERAGE
    __gcov_flush();
    #endif
    exit(1);
mallocerr:
    BP_IGNORE(write(commonfd[PIPE_WRITE], "malloc failure", 15))
    close(commonfd[PIPE_WRITE]);
    #ifdef COVERAGE
    __gcov_flush();
    #endif
    exit(1);
}

Process Process::Builder::ProcessMaster(int pid, int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2])
{
    if (commonfd[PIPE_WRITE] != -1)
        close(commonfd[PIPE_WRITE]);
    if (fdStdOut[PIPE_WRITE] != -1)
        close(fdStdOut[PIPE_WRITE]);
    if (fdStdErr[PIPE_WRITE] != -1)
        close(fdStdErr[PIPE_WRITE]);
    if (fdStdIn[PIPE_READ] != -1)
        close(fdStdIn[PIPE_READ]);

    char buf[4096];
    auto len = read(commonfd[PIPE_READ], buf, 4096);

    if (len > 0)
        throw OSException(buf);
    close(commonfd[PIPE_READ]);
    return (Process(pid, fdStdIn, fdStdOut, fdStdErr));
}
#endif

Process Process::Builder::Build()
{
    auto var = std::move(_argv);
    _argv = Array<String>(var.Size() + 1);
    _argv[0] = std::move(_appName);
    for (fsize i = 0; i != var.Size(); ++i)
        _argv[i + 1] = std::move(var[i]);
    for (auto &a : _argv)
    {
        if (*a == nullptr)
            throw OSException("Detected an attempt to crash memcpy");
    }
    for (auto &kv : _envp)
    {
        if (*kv.Key == nullptr || *kv.Value == nullptr)
            throw OSException("Detected an attempt to crash memcpy");
    }
#ifdef WINDOWS
    if (!_envp.HasKey("SystemRoot"))
    {
        WCHAR buf[MAX_PATH];
        if (!GetSystemDirectoryW(buf, MAX_PATH))
            throw OSException("Could not obtain system root directory");
        _envp["SystemRoot"] = String::FromUTF16(reinterpret_cast<const fchar16 *>(buf));
    }
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;
    HANDLE fdStdOut[2] = {nullptr, nullptr};
    HANDLE fdStdIn[2] = {nullptr, nullptr};
    HANDLE fdStdErr[2] = {nullptr, nullptr};
    if (_redirectStdOut && (!CreatePipe(&fdStdOut[PIPE_READ], &fdStdOut[PIPE_WRITE], &sa, 0) ||
                            !SetHandleInformation(fdStdOut[PIPE_READ], HANDLE_FLAG_INHERIT, 0)))
        throw OSException("Could not create standard output redirection");
    if (_redirectStdErr && (!CreatePipe(&fdStdErr[PIPE_READ], &fdStdErr[PIPE_WRITE], &sa, 0) ||
                            !SetHandleInformation(fdStdErr[PIPE_READ], HANDLE_FLAG_INHERIT, 0)))
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn);
        throw OSException("Could not create standard error redirection");
    }
    if (_redirectStdIn && (!CreatePipe(&fdStdIn[PIPE_READ], &fdStdIn[PIPE_WRITE], &sa, 0) ||
                           !SetHandleInformation(fdStdIn[PIPE_WRITE], HANDLE_FLAG_INHERIT, 0)))
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn);
        throw OSException("Could not create standard input redirection");
    }
    auto appName = _appExe.ToUTF16();
    String cmdLine = "";
    for (auto &a : _argv)
        cmdLine += a + ' ';
    cmdLine = cmdLine.Sub(0, cmdLine.Len() - 1);
    auto u16CmdLine = cmdLine.ToUTF16();
    ArrayList<fchar16> envBlock;
    for (auto &kv : _envp)
    {
        auto wd = (kv.Key + '=' + kv.Value).ToUTF16();
        envBlock += wd;
    }
    envBlock.Add('\0');
    auto envBlockArr = envBlock.ToArray();
    auto curDir = _workDir.PlatformPath().ToUTF16();
    STARTUPINFOW stInfo;
    memset(&stInfo, 0, sizeof(STARTUPINFOW));
    stInfo.cb = sizeof(STARTUPINFOW);
    if (_redirectStdErr || _redirectStdIn || _redirectStdOut)
    {
        stInfo.dwFlags = STARTF_USESTDHANDLES;
        stInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        stInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        stInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }
    if (_redirectStdErr)
        stInfo.hStdError = fdStdErr[PIPE_WRITE];
    if (_redirectStdIn)
        stInfo.hStdInput = fdStdIn[PIPE_READ];
    if (_redirectStdOut)
        stInfo.hStdOutput = fdStdOut[PIPE_WRITE];
    PROCESS_INFORMATION pInfo;
    if (!CreateProcessW(reinterpret_cast<LPCWSTR>(*appName), reinterpret_cast<LPWSTR>(*u16CmdLine), nullptr, nullptr, TRUE,
                        CREATE_UNICODE_ENVIRONMENT | NORMAL_PRIORITY_CLASS, reinterpret_cast<LPVOID>(*envBlockArr),
                        reinterpret_cast<LPCWSTR>(*curDir), &stInfo, &pInfo))
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn);
        throw OSException(String("Could not create process: ") + OSPrivate::ObtainLastErrorString());
    }
    if (fdStdOut[PIPE_WRITE] != nullptr)
    {
        CloseHandle(fdStdOut[PIPE_WRITE]);
        fdStdOut[PIPE_WRITE] = nullptr;
    }
    if (fdStdErr[PIPE_WRITE] != nullptr)
    {
        CloseHandle(fdStdErr[PIPE_WRITE]);
        fdStdErr[PIPE_WRITE] = nullptr;
    }
    if (fdStdIn[PIPE_READ] != nullptr)
    {
        CloseHandle(fdStdIn[PIPE_READ]);
        fdStdIn[PIPE_READ] = nullptr;
    }
    return (Process(&pInfo, fdStdIn, fdStdOut, fdStdErr));
#else
    int fdStdOut[2] = {-1, -1};
    int fdStdIn[2] = {-1, -1};
    int fdStdErr[2] = {-1, -1};
    int commonfd[2];
    if (pipe(commonfd) != 0)
        throw OSException("Could not create common pipe");
    if (_redirectStdOut && pipe(fdStdOut) != 0)
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn, commonfd);
        throw OSException("Could not create standard output redirection");
    }
    if (_redirectStdErr && pipe(fdStdErr) != 0)
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn, commonfd);
        throw OSException("Could not create standard error redirection");
    }
    if (_redirectStdIn && pipe(fdStdIn) != 0)
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn, commonfd);
        throw OSException("Could not create standard input redirection");
    }
    auto pid = fork();
    if (pid == -1)
    {
        CleanupHandles(fdStdOut, fdStdErr, fdStdIn, commonfd);
        throw OSException(String("Could not create process: ") + OSPrivate::ObtainLastErrorString());
    }
    if (pid == 0)
    {
        // Worker/Child
        ProcessWorker(fdStdOut, fdStdErr, fdStdIn, commonfd);
        return (Process(0, fdStdIn, fdStdOut, fdStdErr)); // This will never trigger as it either exits or execve
    }
    else // Master
        return (ProcessMaster(pid, fdStdOut, fdStdErr, fdStdIn, commonfd));
#endif
}

#ifdef WINDOWS
Process::PipeStream::PipeStream(void *pipefd[2])
{
    _pipeHandles[0] = pipefd[0];
    _pipeHandles[1] = pipefd[1];
}

Process::PipeStream::~PipeStream()
{
    if (_pipeHandles[0] != nullptr)
        CloseHandle(_pipeHandles[0]);
    if (_pipeHandles[1] != nullptr)
        CloseHandle(_pipeHandles[1]);
}

Process::PipeStream::PipeStream(PipeStream &&other) noexcept
{
    _pipeHandles[0] = other._pipeHandles[0];
    _pipeHandles[1] = other._pipeHandles[1];
    other._pipeHandles[0] = nullptr;
    other._pipeHandles[1] = nullptr;
}

Process::PipeStream &Process::PipeStream::operator=(PipeStream &&other) noexcept
{
    if (_pipeHandles[0] != nullptr)
        CloseHandle(_pipeHandles[0]);
    if (_pipeHandles[1] != nullptr)
        CloseHandle(_pipeHandles[1]);
    _pipeHandles[0] = other._pipeHandles[0];
    _pipeHandles[1] = other._pipeHandles[1];
    other._pipeHandles[0] = nullptr;
    other._pipeHandles[1] = nullptr;
    return (*this);
}
#else
Process::PipeStream::PipeStream(const int pipefd[2])
{
    _pipfd[0] = pipefd[0];
    _pipfd[1] = pipefd[1];
}

Process::PipeStream::~PipeStream()
{
    if (_pipfd[0] != -1)
        close(_pipfd[0]);
    if (_pipfd[1] != -1)
        close(_pipfd[1]);
}

Process::PipeStream::PipeStream(PipeStream &&other) noexcept
{
    _pipfd[0] = other._pipfd[0];
    _pipfd[1] = other._pipfd[1];
    other._pipfd[0] = -1;
    other._pipfd[1] = -1;
}

Process::PipeStream &Process::PipeStream::operator=(PipeStream &&other) noexcept
{
    if (_pipfd[0] != -1)
        close(_pipfd[0]);
    if (_pipfd[1] != -1)
        close(_pipfd[1]);
    _pipfd[0] = other._pipfd[0];
    _pipfd[1] = other._pipfd[1];
    other._pipfd[0] = -1;
    other._pipfd[1] = -1;
    return (*this);
}
#endif

fsize Process::PipeStream::Read(void *buf, fsize bufsize)
{
#ifdef WINDOWS
    DWORD read;
    if (!ReadFile(_pipeHandles[PIPE_READ], buf, (DWORD)bufsize, &read, nullptr))
        return (0); // ReadFile is broken in this case: instead of just returning 0 bytes when empty instead it errors
    return ((fsize)read);
#else
    int res = read(_pipfd[PIPE_READ], buf, bufsize);
    if (res == -1)
        throw IOException("Cannot read from pipe");
    return ((fsize)res);
#endif
}

fsize Process::PipeStream::Write(const void *buf, fsize bufsize)
{
    // It seems that on both operating systems when the buffer is empty it is considdered as an error
    if (bufsize == 0)
        return (0);
#ifdef WINDOWS
    DWORD written;
    if (!WriteFile(_pipeHandles[PIPE_WRITE], buf, (DWORD)bufsize, &written, nullptr))
        throw IOException("Cannot write to pipe");
    return ((fsize)written);
#else
    int res = write(_pipfd[PIPE_WRITE], buf, bufsize);
    if (res == -1)
        throw IOException("Cannot write to pipe");
    return ((fsize)res);
#endif
}

void Process::PipeStream::Close()
{
#ifdef WINDOWS
    if (_pipeHandles[0] != nullptr)
        CloseHandle(_pipeHandles[0]); //TODO: double close is not handled by winmotherfucker
    if (_pipeHandles[1] != nullptr)
        CloseHandle(_pipeHandles[1]);
    _pipeHandles[0] = nullptr;
    _pipeHandles[1] = nullptr;
#else
    if (_pipfd[0] != -1)
        close(_pipfd[0]);
    if (_pipfd[1] != -1)
        close(_pipfd[1]);
    _pipfd[0] = -1;
    _pipfd[1] = -1;
#endif
}

#ifdef WINDOWS
Process::Process(void *pinfo, void *fdStdIn[2], void *fdStdOut[2], void *fdStdErr[2])
    : _lastExitCode(0)
    , _crashed(false)
    , _running(true)
    , _redirectStdIn(fdStdIn[PIPE_WRITE] != nullptr)
    , _redirectStdOut(fdStdOut[PIPE_READ] != nullptr)
    , _redirectStdErr(fdStdErr[PIPE_READ] != nullptr)
    , _stdIn(fdStdIn)
    , _stdOut(fdStdOut)
    , _stdErr(fdStdErr)
{
    PROCESS_INFORMATION *pInfo = reinterpret_cast<PROCESS_INFORMATION *>(pinfo);
    _pHandle = pInfo->hProcess;
    _tHandle = pInfo->hThread;
}

Process::Process(Process &&other) noexcept
    : _lastExitCode(other._lastExitCode)
    , _crashed(other._crashed)
    , _running(other._running)
    , _redirectStdIn(other._redirectStdIn)
    , _redirectStdOut(other._redirectStdOut)
    , _redirectStdErr(other._redirectStdErr)
    , _stdIn(std::move(other._stdIn))
    , _stdOut(std::move(other._stdOut))
    , _stdErr(std::move(other._stdErr))
    , _pHandle(other._pHandle)
    , _tHandle(other._tHandle)
{
    other._pHandle = nullptr;
    other._tHandle = nullptr;
}

Process &Process::operator=(Process &&other) noexcept
{
    if (_pHandle != nullptr && _tHandle != nullptr)
    {
        if (_running)
            TerminateProcess(_pHandle, 1);
        CloseHandle(_pHandle);
        CloseHandle(_tHandle);
    }
    _lastExitCode = other._lastExitCode;
    _crashed = other._crashed;
    _running = other._running;
    _redirectStdIn = other._redirectStdIn;
    _redirectStdOut = other._redirectStdOut;
    _redirectStdErr = other._redirectStdErr;
    _stdIn = std::move(other._stdIn);
    _stdOut = std::move(other._stdOut);
    _stdErr = std::move(other._stdErr);
    _pHandle = other._pHandle;
    _tHandle = other._tHandle;
    other._pHandle = nullptr;
    other._tHandle = nullptr;
    return (*this);
}
#else
Process::Process(int pid, int fdStdIn[2], int fdStdOut[2], int fdStdErr[2])
    : _lastExitCode(0)
    , _crashed(false)
    , _running(true)
    , _redirectStdIn(fdStdIn[PIPE_WRITE] != -1)
    , _redirectStdOut(fdStdOut[PIPE_READ] != -1)
    , _redirectStdErr(fdStdErr[PIPE_READ] != -1)
    , _stdIn(fdStdIn)
    , _stdOut(fdStdOut)
    , _stdErr(fdStdErr)
    , _pid(pid)
{
}

Process::Process(Process &&other) noexcept
    : _lastExitCode(other._lastExitCode)
    , _crashed(other._crashed)
    , _running(other._running)
    , _redirectStdIn(other._redirectStdIn)
    , _redirectStdOut(other._redirectStdOut)
    , _redirectStdErr(other._redirectStdErr)
    , _stdIn(std::move(other._stdIn))
    , _stdOut(std::move(other._stdOut))
    , _stdErr(std::move(other._stdErr))
    , _pid(other._pid)
{
    other._pid = -1;
}

Process &Process::operator=(Process &&other) noexcept
{
    if (_pid != -1 && _running)
        kill(_pid, SIGKILL);
    _lastExitCode = other._lastExitCode;
    _crashed = other._crashed;
    _running = other._running;
    _redirectStdIn = other._redirectStdIn;
    _redirectStdOut = other._redirectStdOut;
    _redirectStdErr = other._redirectStdErr;
    _stdIn = std::move(other._stdIn);
    _stdOut = std::move(other._stdOut);
    _stdErr = std::move(other._stdErr);
    _pid = other._pid;
    other._pid = -1;
    return (*this);
}
#endif

Process::~Process()
{
#ifdef WINDOWS
    if (_pHandle != nullptr && _tHandle != nullptr)
    {
        if (_running)
            TerminateProcess(_pHandle, 1);
        CloseHandle(_pHandle);
        CloseHandle(_tHandle);
    }
#else
    if (_pid != -1 && _running)
        kill(_pid, SIGKILL);
#endif
}

void Process::Wait()
{
#ifdef WINDOWS
    DWORD res = WaitForSingleObject(_pHandle, UInt::MaxValue);
    if (res != 0)
        throw OSException(String("Error waiting process termination: ") + OSPrivate::ObtainLastErrorString());
    _lastExitCode = (uint32)-1;
    GetExitCode();
#else
    if (waitpid(_pid, &_lastExitCode, 0) == -1)
        throw OSException(String("Error waiting process termination: ") + OSPrivate::ObtainLastErrorString());
    _lastExitCode = WIFSIGNALED(_lastExitCode) ? WTERMSIG(_lastExitCode) : WEXITSTATUS(_lastExitCode);
    _crashed = WIFSIGNALED(_lastExitCode);
    _running = false;
#endif
}

#ifdef WINDOWS
static std::set<DWORD> EnumerateWindowThreads(DWORD pid)
{
    std::set<DWORD> threads;
    for (HWND hwnd = GetTopWindow(nullptr); hwnd; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT))
    {
        DWORD pidw;
        DWORD tid = GetWindowThreadProcessId(hwnd, &pidw);
        if (pidw == pid)
            threads.emplace(tid);
    }
    return threads;
}
#endif

void Process::Kill(bool force)
{
    if (!_running)
        return;
#ifdef WINDOWS
    if (!force)
    {
        DWORD pid = GetProcessId(_pHandle);
        if (AttachConsole(pid))
        {
            SetConsoleCtrlHandler(nullptr, true); // Disable Ctrl-C handling for our program
            GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
            WaitForSingleObject(_pHandle, 1000);
            FreeConsole();
            SetConsoleCtrlHandler(nullptr, false); // Re-enable Ctrl-C handling
        }
        else // Windows rejected attaching the console so we assume it's a window so we try to kill the windows
        {
            auto windows = EnumerateWindowThreads(pid);
            if (windows.size() == 0)
            {
                // Fact is it's definatly not a window based app so assume Windows is a peace of shit and unable to
                // attach a process which does have a console Rely on force kill then
                Kill(true);
                return;
            }
            for (DWORD tid : windows)
            {
                if (!PostThreadMessage(tid, WM_QUIT, 0, 0))
                    throw OSException("Error trying to send WM_QUIT message");
            }
        }
    }
    else
    {
        if (!TerminateProcess(_pHandle, 1))
            throw OSException("Could not terminate process");
        _lastExitCode = (uint32)-1;
        GetExitCode();
    }
#else
    if (kill(_pid, force ? SIGKILL : SIGINT) == -1)
        throw OSException("Could not terminate process");
    // It seem that under Linux kill does not wait until the signal has been sent so attempt to emulate the expected
    // behaviour with a sleep
    Thread::Sleep(100);
    GetExitCode();
#endif
}

uint32 Process::GetExitCode()
{
#ifdef WINDOWS
    if (_lastExitCode != (uint32)-1)
        return (_lastExitCode);
    DWORD res;
    if (!GetExitCodeProcess(_pHandle, &res))
        throw OSException(String("Could not poll target process: ") + OSPrivate::ObtainLastErrorString());
    _lastExitCode = res;
    if (_lastExitCode != STILL_ACTIVE)
    {
        _crashed = _lastExitCode > 0x80000000 || _lastExitCode == 3;
        _running = false;
    }
    return (_lastExitCode);
#else
    if (!_running)
        return (_lastExitCode);
    int res = waitpid(_pid, &_lastExitCode, WNOHANG);
    if (res == -1)
        throw OSException(String("Could not poll target process: ") + OSPrivate::ObtainLastErrorString());
    if (res != 0)
    {
        _lastExitCode = WIFSIGNALED(_lastExitCode) ? WTERMSIG(_lastExitCode) : WEXITSTATUS(_lastExitCode);
        _running = false;
    }
    return ((uint32)_lastExitCode);
#endif
}

Process::PipeStream &Process::GetStandardInput()
{
    if (!_redirectStdIn)
        throw OSException("The target process does not allow standard input redirection");
    return (_stdIn);
}

Process::PipeStream &Process::GetStandardOutput()
{
    if (!_redirectStdOut)
        throw OSException("The target process does not allow standard output redirection");
    return (_stdOut);
}

Process::PipeStream &Process::GetStandardError()
{
    if (!_redirectStdErr)
        throw OSException("The target process does not allow standard error redirection");
    return (_stdErr);
}
