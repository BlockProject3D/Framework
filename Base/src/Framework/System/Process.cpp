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
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

using namespace bpf;
using namespace bpf::io;
using namespace bpf::system;

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
            _appExe = f.PlatformPath();
        else
            throw IOException(String("File '") + name + "' does not exist");
    }
    else
    {
        auto path = _envp["PATH"];
        if (path.Size() == 0)
            path = PROCESS_DEFAULT_PATH;
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

Process Process::Builder::Build()
{
#ifdef WINDOWS
//TODO: Check SystemRoot env variable if none auto specify
//TODO: CreatePipe
//TODO: CreateProcess
#else
    int fdStdOut[2] = {-1, -1};
    int fdStdIn[2] = {-1, -1};
    int fdStdErr[2] = {-1, -1};
    int commonfd[2];
    if (pipe(commonfd) != 0)
        throw OSException("Could not create common pipe");
    close(commonfd[PIPE_WRITE]);
    if (_redirectStdOut && pipe(fdStdOut) != 0)
        throw OSException("Could not create standard output redirection");
    if (_redirectStdErr && pipe(fdStdErr) != 0)
        throw OSException("Could not create standard error redirection");
    if (_redirectStdIn && pipe(fdStdIn) != 0)
        throw OSException("Could not create standard input redirection");
    close(fdStdOut[PIPE_WRITE]);
    close(fdStdErr[PIPE_WRITE]);
    close(fdStdIn[PIPE_READ]);
    auto pid = fork();
    if (pid == -1)
        throw OSException("Could not create child process");
    if (pid == 0)
    {
        //Master
        //TODO: Alloc process instance
        char buf[4096];
        auto len = read(commonfd[PIPE_READ], buf, 4096);
        if (len > 0)
            throw OSException(buf);
        //TODO: Return Process instance
    }
    else
    {
        //Worker/Child
        for (int fd = 0; fd != 256; ++fd)
        {
            if (fd != fdStdOut[PIPE_WRITE] && fd != fdStdErr[PIPE_WRITE] && fd != fdStdIn[PIPE_READ] && fd != commonfd[PIPE_WRITE])
                close(fd);
        }
        if (fcntl(commonfd[PIPE_WRITE], FD_CLOEXEC) != 0)
        {
            write(commonfd[PIPE_WRITE], "fcntl failure", 14);
            close(commonfd[PIPE_WRITE]);
            exit(1);
        }
        if (_redirectStdOut && dup2(2, fdStdOut[PIPE_WRITE]) == -1)
            goto redirecterr;
        if (_redirectStdErr && dup2(2, fdStdErr[PIPE_WRITE]) == -1)
            goto redirecterr;
        if (_redirectStdIn && dup2(0, fdStdIn[PIPE_READ]) == -1)
            goto redirecterr;
        goto next;
redirecterr:
        write(commonfd[PIPE_WRITE], "Could not create one or more redirection(s)", 44);
        close(commonfd[PIPE_WRITE]);
        exit(1);
next:
        chdir(*_workDir.PlatformPath());
        
    }
#endif
}
