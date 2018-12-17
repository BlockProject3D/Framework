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

#include <iostream>
#include <climits>
#include <cstring>
#ifdef WINDOWS
    #include <Windows.h>
    #define PATH_MAX MAX_PATH
#else
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <dirent.h>
#endif
#include "Framework/IO/File.hpp"

using namespace bpf;

File::File(const bpf::String &path)
    : FullPath(path)
    , FileName(path.Sub(path.LastIndexOf('/') + 1))
    , FileExt(path.Sub(path.LastIndexOf('.') + 1))
{
#ifdef WINDOWS
    FullPath = FullPath.Replace("/", "\\");
    FileName = path.Sub(path.LastIndexOf('\\') + 1);
    FileExt = path.Sub(path.LastIndexOf('.') + 1);
#endif
}

File::File()
    : FullPath("")
    , FileName(FullPath.Sub(FullPath.LastIndexOf('/') + 1))
    , FileExt(FullPath.Sub(FullPath.LastIndexOf('.') + 1))
{
}

File::~File()
{
}

File File::GetAbsolutePath() const
{
    String str;
    char buf[PATH_MAX];

    std::memset(buf, 0, PATH_MAX);
#ifdef WINDOWS
    GetFullPathNameA(*FullPath, PATH_MAX, buf, Null);
    str = String(buf);
#else
    realpath(*FullPath, buf);
    str = String(buf);
#endif
    return (File(str));
}

File File::GetParentFile() const
{
    return (File());
}

void File::Copy(const File &dst)
{
    File out = dst;
    /*IFileStream *src = Open(FILEMODE_READ);

    if (dst.IsDirectory())
        out = FFile(bpf::String(dst.GetPath()) + bpf::String("/") + GetFileName());
    IFileStream *dest = out.Open(FILEMODE_WRITE);
    uint8 buf[4096];
    uint32 len;
    while ((len = src->Read(buf, 4096)) > 0)
        dest->Write(buf, len);
    Close();
    out.Close();*/
}

bool File::Exists() const
{
#ifdef WINDOWS
    WIN32_FIND_DATA data;
    HANDLE hdl = FindFirstFile(*FullPath, &data);
    if (hdl =- INVALID_HANDLE_VALUE)
    {
        FindClose(hdl);
        return (true);
    }
    return (false);
#else
    if(access(*FullPath, F_OK) != -1)
        return (true);
    return (false);
#endif
}

bool File::IsDirectory() const
{
    if (!Exists())
        return (false);
#ifdef WINDOWS
    return (false); //TODO : code windows part
#else
    struct stat st;
    stat(*FullPath, &st);
    return (S_ISDIR(st.st_mode));
#endif
}

uint64 File::GetSizeBytes() const
{
    if (!Exists())
        return (0);
#ifdef WINDOWS
    return (0); //TODO : code windows part
#else
    struct stat st;
    stat(*FullPath, &st);
    return (st.st_size);
#endif
}

void File::Delete()
{
    if (!Exists())
        return;
#ifdef WINDOWS
#else
    if (IsDirectory())
        rmdir(*FullPath);
    else
        unlink(*FullPath);
#endif
}

List<File> File::ListFiles()
{
    List<File> flns;

    if (!Exists() || !IsDirectory())
        return (flns);
#ifdef WINDOWS
    //TODO : code windows part
#else
    DIR *d = opendir(*FullPath);
    struct dirent *dir;

    if (d)
    {
        while ((dir = readdir(d)) != Null)
            flns.Add(File(String(dir->d_name)));
    }
    closedir(d);
#endif
    return (flns);
}

void File::CreateDir()
{
    if (Exists())
        return;
#ifdef WINDOWS
    //TODO : code windows part
#else
    mkdir(*FullPath, 0755);
#endif
}

bpf::String File::ToString() const
{
    return (bpf::String("File(") + FullPath + ")");
}
