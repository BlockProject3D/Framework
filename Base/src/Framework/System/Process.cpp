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
#include "Framework/IO/IOException.hpp"
#include "Framework/System/OSException.hpp"
#ifdef WINDOWS
    #include <Windows.h>
    #include <set>
#else
    #include <fcntl.h>
    #include <string.h>
    #include <unistd.h>
    #include <wait.h>
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
    if (name.Contains('/'))
    {
        auto f = File(name);
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
            auto f = File(p) + File(name);
            if (f.Exists() && !f.IsDirectory())
            {
                _appExe = f.PlatformPath();
                _appName = f.Name();
                break;
            }
        }
        if (_appExe == String::Empty)
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

#ifndef WINDOWS
void Process::Builder::ProcessWorker(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2])
{
    char **argv = reinterpret_cast<char **>(malloc(sizeof(char **) * (_argv.Size() + 1)));
    char **envp = reinterpret_cast<char **>(malloc(sizeof(char **) * (_envp.Size() + 1)));
    fsize i = 0;

    if (argv == NULL || envp == NULL)
        goto mallocerr;
    for (int fd = 0; fd != 256; ++fd)
    {
        if (fd != 0 && fd != 1 && fd != 2 && fd != fdStdOut[PIPE_WRITE] && fd != fdStdErr[PIPE_WRITE] &&
            fd != fdStdIn[PIPE_READ] && fd != commonfd[PIPE_WRITE])
            close(fd);
    }
    if (fcntl(commonfd[PIPE_WRITE], FD_CLOEXEC) != 0)
    {
        write(commonfd[PIPE_WRITE], "fcntl failure", 14);
        close(commonfd[PIPE_WRITE]);
        exit(1);
    }
    if (_redirectStdOut && dup2(fdStdOut[PIPE_WRITE], 1) == -1)
        goto redirecterr;
    if (_redirectStdErr && dup2(fdStdErr[PIPE_WRITE], 2) == -1)
        goto redirecterr;
    if (_redirectStdIn && dup2(fdStdIn[PIPE_READ], 0) == -1)
        goto redirecterr;
    chdir(*_workDir.PlatformPath());
    for (auto &a : _argv)
    {
        argv[i] = reinterpret_cast<char *>(malloc(a.Size() + 1));
        if (argv[i] == NULL)
            goto mallocerr;
        memcpy(argv[i], *a, a.Size() + 1); // Copy with additional '\0'
        ++i;
    }
    argv[i] = NULL;
    i = 0;
    for (auto &kv : _envp)
    {
        envp[i] = reinterpret_cast<char *>(malloc(kv.Key.Size() + kv.Value.Size() + 2));
        if (envp[i] == NULL)
            goto mallocerr;
        memcpy(envp[i], *kv.Key, kv.Key.Size());
        envp[i][kv.Key.Size()] = '=';
        memcpy(envp[i] + kv.Key.Size() + 1, *kv.Value, kv.Value.Size() + 1);
        ++i;
    }
    envp[i] = NULL;
    if (execve(*_appExe, argv, envp) == -1)
    {
        write(commonfd[PIPE_WRITE], "execve failure", 15);
        close(commonfd[PIPE_WRITE]);
        exit(1);
    }
redirecterr:
    write(commonfd[PIPE_WRITE], "Could not create one or more redirection(s)", 44);
    close(commonfd[PIPE_WRITE]);
    exit(1);
mallocerr:
    write(commonfd[PIPE_WRITE], "malloc failure", 15);
    close(commonfd[PIPE_WRITE]);
    exit(1);
}

Process Process::Builder::ProcessMaster(int pid, int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2])
{
    close(commonfd[PIPE_WRITE]);
    close(fdStdOut[PIPE_WRITE]);
    close(fdStdErr[PIPE_WRITE]);
    close(fdStdIn[PIPE_READ]);

    Process p(pid, fdStdIn, fdStdOut, fdStdErr);
    char buf[4096];
    auto len = read(commonfd[PIPE_READ], buf, 4096);

    if (len > 0)
        throw OSException(buf);
    close(commonfd[PIPE_READ]);
    return (p);
}
#endif

Process Process::Builder::Build()
{
    auto var = std::move(_argv);
    _argv = Array<String>(var.Size() + 1);
    _argv[0] = std::move(_appName);
    for (fsize i = 1; i < var.Size(); ++i)
    {
        _argv[i] = std::move(var[i]);
        ++i;
    }
    for (auto &a : _argv)
    {
        if (*a == Null)
            throw OSException("Detected an attempt to crash memcpy");
    }
    for (auto &kv : _envp)
    {
        if (*kv.Key == Null || *kv.Value == Null)
            throw OSException("Detected an attempt to crash memcpy");
    }
#ifdef WINDOWS
// TODO: Check SystemRoot env variable if none auto specify
// TODO: CreatePipe
// TODO: CreateProcess
// TODO: PROCESS_QUERY_INFORMATION
#else
    int fdStdOut[2] = {-1, -1};
    int fdStdIn[2] = {-1, -1};
    int fdStdErr[2] = {-1, -1};
    int commonfd[2];
    if (pipe(commonfd) != 0)
        throw OSException("Could not create common pipe");
    if (_redirectStdOut && pipe(fdStdOut) != 0)
        throw OSException("Could not create standard output redirection");
    if (_redirectStdErr && pipe(fdStdErr) != 0)
        throw OSException("Could not create standard error redirection");
    if (_redirectStdIn && pipe(fdStdIn) != 0)
        throw OSException("Could not create standard input redirection");
    auto pid = fork();
    if (pid == -1)
        throw OSException("Could not create child process");
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

Process::PStream::PStream(int pipefd[2])
{
#ifdef WINDOWS
#else
    _pipfd[0] = pipefd[0];
    _pipfd[1] = pipefd[1];
#endif
}

Process::PStream::~PStream()
{
#ifdef WINDOWS
#else
    close(_pipfd[0]);
    close(_pipfd[1]);
#endif
}

fsize Process::PStream::Read(void *buf, fsize bufsize)
{
#ifdef WINDOWS
#else
    return (read(_pipfd[PIPE_READ], buf, bufsize));
#endif
}

fsize Process::PStream::Write(const void *buf, fsize bufsize)
{
#ifdef WINDOWS
#else
    return (write(_pipfd[PIPE_WRITE], buf, bufsize));
#endif
}

#ifdef WINDOWS
#else
Process::Process(int pid, int fdStdIn[2], int fdStdOut[2], int fdStdErr[2])
    : _lastExitCode(-1)
    , _redirectStdIn(fdStdIn[PIPE_WRITE] != -1)
    , _redirectStdOut(fdStdOut[PIPE_READ] != -1)
    , _redirectStdErr(fdStdErr[PIPE_READ] != -1)
    , _stdIn(fdStdIn)
    , _stdOut(fdStdOut)
    , _stdErr(fdStdErr)
    , _pid(pid)
{
}
#endif

Process::~Process()
{
#ifdef WINDOWS
#else
    if (_lastExitCode == -1)
        kill(_pid, SIGKILL);
#endif
}

void Process::Wait()
{
#ifdef WINDOWS
    if (!WaitForSingleObject(_handle))
        throw OSException("Error waiting process termination");
#else
    if (waitpid(_pid, &_lastExitCode, 0) == -1)
        throw OSException("Error waiting process termination");
    _lastExitCode = WEXITSTATUS(_lastExitCode);
#endif
}

#ifdef WINDOWS
static std::set<DWORD> EnumerateWindowThreads(DWORD pid)
{
    std::set<DWORD> threads;
    for (HWND hwnd = GetTopWindow(NULL); hwnd; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT))
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
    if (_lastExitCode != -1)
        return;
#ifdef WINDOWS
    if (!force)
    {
        DWORD pid = GetProcessId(_handle);
        if (AttachConsole(pid))
        {
            SetConsoleCtrlHandler(NULL, true); // Disable Ctrl-C handling for our program
            GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
            WaitForSingleObject(hProcess, 1000);
            FreeConsole();
            SetConsoleCtrlHandler(NULL, false); // Re-enable Ctrl-C handling
        }
        else
        {
            for (DWORD tid : EnumerateWindowThreads(pid))
            {
                if (!PostThreadMessage(tid, WM_QUIT, 0, 0))
                    throw OSException("Error trying to send WM_QUIT message");
            }
        }
    }
    else
    {
        if (!TerminateProcess(_handle, 1))
            throw OSException("Could not terminate process");
    }
#else
    if (kill(_pid, force ? SIGKILL : SIGINT) == -1)
        throw OSException("Could not terminate process");
#endif
}

fint Process::GetExitCode()
{
    if (_lastExitCode != -1)
        return (_lastExitCode);
#ifdef WINDOWS
    if (!GetExitCodeProcess(_handle, &_lastExitCode))
        throw OSException("Could not poll target process");
    if (_lastExitCode == STILL_RUNNING)
        _lastExitCode = -1;
#else
    int res = waitpid(_pid, &_lastExitCode, WNOHANG);
    if (res == -1)
        throw OSException("Could not poll target process");
    if (res == 0)
        _lastExitCode = -1;
    else
        _lastExitCode = WEXITSTATUS(_lastExitCode);
#endif
    return (_lastExitCode);
}

IOutputStream &Process::GetStandardInput()
{
    if (!_redirectStdIn)
        throw OSException("The target process does not allow standard input redirection");
    return (_stdIn);
}

IInputStream &Process::GetStandardOutput()
{
    if (!_redirectStdOut)
        throw OSException("The target process does not allow standard output redirection");
    return (_stdOut);
}

IInputStream &Process::GetStandardError()
{
    if (!_redirectStdErr)
        throw OSException("The target process does not allow standard error redirection");
    return (_stdErr);
}
