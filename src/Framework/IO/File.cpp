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
    , FileName("")
    , FileExt("")
{
#ifdef WINDOWS
    FullPath = FullPath.Replace("/", "\\");
    String result = String::Empty;
    char old = '\0';
    for (int i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '\\' && old != '\\')
            result += '\\';
        else
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '\\')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    FileName = path.Sub(path.LastIndexOf('\\') + 1);
    FileExt = path.Sub(path.LastIndexOf('.') + 1);
#else
    String result = String::Empty;
    char old = '\0';
    for (int i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '/' && old != '/')
            result += '/';
        else
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '/')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    FileName = path.Sub(path.LastIndexOf('/') + 1);
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

File File::GetParent() const
{
    return (File(FullPath.Sub(0, FullPath.LastIndexOf('/'))));
}

void File::Copy(const File &dst, bool overwrite)
{
#ifdef WINDOWS
    CopyFile(*GetAbsolutePath().GetPath(), *dst.GetAbsolutePath().GetPath(), !overwrite);
#else
#endif
    //TODO : Implement
    //File out = dst;
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
    DWORD attr = GetFileAttributes(*FullPath);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return (false);
    return (true);
#else
    if(access(*FullPath, F_OK) != -1)
        return (true);
    return (false);
#endif
}

bool File::IsHidden() const
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributes(*FullPath);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return (false);
    return (attr & FILE_ATTRIBUTE_HIDDEN);
#else
    return (FileName[0] == '.');
#endif
}

void File::Hide(const bool flag)
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributes(*FullPath);
    if (flag)
        attr |= FILE_ATTRIBUTE_HIDDEN;
    else
        attr &= ~FILE_ATTRIBUTE_HIDDEN;
    SetFileAttributes(*FullPath, attr);
#else
    File f = GetParent().Path() + "/" + "." + Name();
    rename(*FullPath, *f.Path());
    FileName = f.Name();
    FullPath = f.Path();
    FileExt = f.Extension();
#endif
}

bool File::IsDirectory() const
{
    if (!Exists())
        return (false);
#ifdef WINDOWS
    DWORD attr = GetFileAttributes(*FullPath);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return (false);
    return (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(*FullPath, &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
#endif
}

uint64 File::GetSizeBytes() const
{
    if (!Exists())
        return (0);
#ifdef WINDOWS
    WIN32_FIND_DATA data;
    HANDLE hdl = FindFirstFile(*FullPath, &data);
    if (hdl == INVALID_HANDLE_VALUE)
        return (0);
    FindClose(hdl);
    return (data.nFileSizeLow | (uint64)data.nFileSizeHigh << 32);
#else
    struct stat st;
    if (stat(*FullPath, &st))
        return (0);
    return (st.st_size);
#endif
}

void File::Delete()
{
    if (!Exists())
        return;
#ifdef WINDOWS
    if (IsDirectory())
        RemoveDirectory(*FullPath);
    else
        DeleteFile(*FullPath);
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
    WIN32_FIND_DATA data;
    File dir = (*this + "/*").GetAbsolutePath();
    HANDLE hdl = FindFirstFile(*dir.Path(), &data);
    if (hdl != INVALID_HANDLE_VALUE)
    {
        flns.Add(File(String(data.cFileName)));
        while (FindNextFile(hdl, &data))
            flns.Add(File(Path() + "/" + String(data.cFileName)));
        FindClose(hdl);
    }
#else
    DIR *d = opendir(*FullPath);
    struct dirent *dir;

    if (d)
    {
        while ((dir = readdir(d)) != Null)
            flns.Add(File(FullPath + "/" + String(dir->d_name)));
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
    CreateDirectory(*FullPath, Null);
#else
    mkdir(*FullPath, 0755);
#endif
}

bpf::String File::ToString() const
{
    return (bpf::String("File(") + FullPath + ")");
}
