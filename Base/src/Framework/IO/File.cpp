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

using namespace bpf::io;
using namespace bpf::collection;
using namespace bpf;

File::File(const bpf::String &path)
    : FullPath(path)
    , UserPath(path)
    , FileName("")
    , FileExt("")
{
#ifdef WINDOWS
    FullPath = FullPath.Replace('/', '\\');
    String result = String::Empty;
    char old = '\0';
    for (fisize i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '\\' && old != '\\')
            result += '\\';
        else if (FullPath.ByteAt(i) != '\\')
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '\\')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    UserPath = FullPath.Replace('\\', '/');
    FileName = path.Sub(path.LastIndexOf('\\') + 1);
    FileExt = path.Sub(path.LastIndexOf('.') + 1);
#else
    String result = String::Empty;
    char old = '\0';
    for (fisize i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '/' && old != '/')
            result += '/';
        else if (FullPath.ByteAt(i) != '/')
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '/')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    UserPath = FullPath;
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

#ifdef WINDOWS
    WCHAR buf[PATH_MAX];
    std::memset(buf, 0, PATH_MAX);
    GetFullPathNameW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), PATH_MAX, buf, Null);
    str = String::FromUTF16(reinterpret_cast<const bpf::fchar16 *>(buf));
#else
    char buf[PATH_MAX];
    std::memset(buf, 0, PATH_MAX);
    realpath(*FullPath, buf);
    str = String(buf);
#endif
    return (File(str));
}

File File::GetParent() const
{
    return (File(FullPath.Sub(0, FullPath.LastIndexOf('/'))));
}

bool File::CopyTo(const File &dst, bool overwrite)
{
#ifdef WINDOWS
    BOOL val = CopyFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), reinterpret_cast<LPCWSTR>(*dst.FullPath.ToUTF16()), !overwrite);
    return (val == TRUE ? true : false);
#else
    return (false);
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

bool File::MoveTo(const File &dst)
{
#ifdef WINDOWS
    BOOL val = MoveFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), reinterpret_cast<LPCWSTR>(*dst.FullPath.ToUTF16()));
    return (val == TRUE ? true : false);
#else
    int val = rename(*FullPath, *dst.FullPath);
    return (val == 0 ? true : false);
#endif
}

bool File::Exists() const
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
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
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
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
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    if (flag)
        attr |= FILE_ATTRIBUTE_HIDDEN;
    else
        attr &= ~FILE_ATTRIBUTE_HIDDEN;
    SetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), attr);
#else
    File f = File(GetParent().Path() + "/" + "." + Name());
    rename(*FullPath, *f.Path());
    FileName = f.Name();
    FullPath = f.PlatformPath();
    UserPath = f.Path();
    FileExt = f.Extension();
#endif
}

bool File::IsDirectory() const
{
    if (!Exists())
        return (false);
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
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
    WIN32_FIND_DATAW data;
    HANDLE hdl = FindFirstFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), &data);
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
        RemoveDirectoryW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    else
        DeleteFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
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
    WIN32_FIND_DATAW data;
    File dir = (*this + "/*");
    HANDLE hdl = FindFirstFileW(reinterpret_cast<LPCWSTR>(*dir.FullPath.ToUTF16()), &data);
    if (hdl != INVALID_HANDLE_VALUE)
    {
        flns.Add(File(FullPath + "/" + String::FromUTF16(reinterpret_cast<const fchar16 *>(data.cFileName))));
        while (FindNextFileW(hdl, &data))
            flns.Add(File(FullPath + "/" + String::FromUTF16(reinterpret_cast<const fchar16 *>(data.cFileName))));
        FindClose(hdl);
    }
#else
    DIR *d = opendir(*FullPath);

    if (d)
    {
        struct dirent *dir;
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
    CreateDirectoryW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), Null);
#else
    mkdir(*FullPath, 0755);
#endif
}
